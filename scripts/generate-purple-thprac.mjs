// Extract the native purple hotkey widget; platform formatting is the only
// substitution. Never silently accept changed native source boundaries.
import { readFileSync, writeFileSync } from 'node:fs';
import { resolve } from 'node:path';
import { createHash } from 'node:crypto';
import assert from 'node:assert/strict';

const root = resolve(import.meta.dirname, '..');
const upstream = process.argv.slice(2).find(arg => !arg.startsWith('--'));
assert(upstream, 'Pass the purple thprac checkout explicitly');
const read = file => readFileSync(resolve(upstream, file), 'utf8').replace(/^\uFEFF/, '').replaceAll('\r\n', '\n');
const sha = value => createHash('sha256').update(value).digest('hex');
const source = read('thprac/src/thprac/thprac_th07.cpp');
assert.equal(sha(source), 'b5be6dc698f574992ab49696c5c0d8ad756af93bf0760be1504ee9e958bee1b0',
  'The TH07 extraction requires the reviewed purple revision; audit changes before changing this pin');
const components = read('thprac/src/thprac/thprac_gui_components.cpp');
const definitions = JSON.parse(read('thprac/src/thprac/thprac_games_def.json'));
const glossary = Object.assign({}, ...Object.values(definitions).map(game => game.glossary ?? {}));
const start = components.indexOf('        bool GuiHotKey::OnWidgetUpdate()');
const end = components.indexOf('        bool GuiHotKey::operator()', start);
assert(start >= 0 && end > start);
let widget = components.slice(start, end).trimEnd();
widget = widget.replace('bool GuiHotKey::OnWidgetUpdate()', 'inline bool PurpleHotkeyWidget(const char* mKeyText, const char* text, bool mStatus)');
widget = widget.replace('            const char* text = mText ? mText : LocaleGetStr(mTextRef);\n', '');
widget = widget.replace('std::format("[{}: {}]", mKeyText, text)', 'std::string("[") + mKeyText + ": " + text + "]"');
widget = widget.replace('std::format("{}: {}", mKeyText, text)', 'std::string(mKeyText) + ": " + text');
assert(!/std::format|LocaleGetStr|mTextRef|GuiHotKey::/.test(widget));
const labelKeys = ['THPRAC_INGAMEINFO', 'TH_ENEMY_MUTEKI', 'TH_ADV_DISABLE_X_KEY', 'TH_ADV_DISABLE_SHIFT_KEY',
  'TH_ADV_DISABLE_Z_KEY', 'TH_ADV_DISABLE_C_KEY_SAMETIME', 'TH_ADV_FORCE_SHIFT_KEY', 'THPRAC_INFLIVES_MAP'];
const labels = labelKeys.map(key => {
  assert(Array.isArray(glossary[key]) && glossary[key].length === 3, key);
  return `inline constexpr const char* purple_${key}[3]{${glossary[key].map(value => JSON.stringify(value)).join(',')}};`;
}).join('\n');
const banner = `// Generated from purple THPrac (MIT), TH07 source sha256 ${sha(source)}.\n`;
const header = banner + '#pragma once\n#include <imgui.h>\n#include <string>\nnamespace PracticeRuntime {\n' +
  widget + '\ninline void DrawPurpleHotkey(const char* key, const char* text, bool& status) {\n' +
  '    if (PurpleHotkeyWidget(key, text, status)) status = !status;\n}\n' + labels + '\n}\n';
const hooksSource = read('thprac/src/thprac/thprac_games_hooks.cpp');
const inputStart = hooksSource.indexOf('    if (g_input_opt.disable_xkey) {');
const inputEnd = hooksSource.indexOf('    if (g_input_opt.disable_f10_11_13)', inputStart);
assert(inputStart >= 0 && inputEnd > inputStart);
let input = hooksSource.slice(inputStart, inputEnd).trimEnd();
const scans = { X: 88, C: 67, Z: 90, LSHIFT: 160, RSHIFT: 161 };
input = input.replaceAll('g_input_opt.', '').replaceAll('((BYTE*)state)', 'state')
  .replace(/DIK_([A-Z]+)/g, (_, key) => { assert(key in scans); return scans[key]; });
assert(!/DIK_|BYTE|g_input_opt/.test(input));
const inputHeader = banner + '#pragma once\n#include <cstdint>\nnamespace PracticeRuntime {\nstruct PurpleInput {\n' +
  ' bool disable_xkey=false,disable_Ckey_at_same_time=true,disable_shiftkey=false,force_shiftkey=false,disable_zkey=false;\n' +
  ' void apply(std::uint8_t* state) const {\n' + input + '\n }\n' +
  ' std::uint16_t filter(std::uint16_t buttons, bool enabled, bool replay) const {\n' +
  '  if (!enabled || replay) return buttons;\n' +
  '  std::uint8_t state[256]{}; state[90]=(buttons&1)?0x80:0;state[88]=(buttons&2)?0x80:0;\n' +
  '  state[160]=state[161]=(buttons&4)?0x80:0; apply(state);\n' +
  '  return (buttons & ~std::uint16_t(7)) | (state[90]?1:0) | (state[88]?2:0) | ((state[160]||state[161])?4:0);\n' +
  ' }\n};\n}\n';
const hooks = [...source.matchAll(/\b(EHOOK_(?:DY|ST)|PATCH_DY)\((\w+),\s*(0x[0-9a-fA-F]+)/g)]
  .map(([, kind, name, address]) => ({ kind, name, address }));
const advStart = source.indexOf('    class THAdvOptWnd');
const advEnd = source.indexOf('    EHOOK_ST(th07_rb', advStart);
assert(advStart >= 0 && advEnd > advStart);
const advanced = source.slice(advStart, advEnd);
const calls = [...new Set([...advanced.matchAll(/\b(\w+Opt)\(/g)].map(match => match[1]))];
for (const required of ['GameFPSOpt', 'DisableKeyOpt', 'KeyHUDOpt', 'InfLifeOpt', 'GameplayOpt', 'InGameReactionTestOpt', 'AboutOpt'])
  assert(calls.includes(required), required);
const inventory = {
  schema: 'th07/purple-source-inventory/1', sourceSha256: sha(source),
  widgetSourceSha256: sha(components), hooks, advancedCalls: calls,
  sections: Object.keys(definitions.th07.sections),
};
const files = {
  'src/PurpleHotkey.hpp': header,
  'src/PurpleInput.hpp': inputHeader,
  'src/PurpleLicense.hpp': banner + '#pragma once\nnamespace PracticeRuntime { inline constexpr const char* purple_license = ' + JSON.stringify(read('LICENCE')) + '; }\n',
  'audit/source-contracts/th07-purple-inventory.json': JSON.stringify(inventory, null, 2) + '\n',
};
for (const [file, contents] of Object.entries(files)) {
  const normalized = contents.split('\n').map(line => line.trimEnd()).join('\n');
  if (process.argv.includes('--write')) writeFileSync(resolve(root, file), normalized);
  else {
    assert(process.argv.includes('--check'), 'Use --write or --check');
    assert.equal(readFileSync(resolve(root, file), 'utf8').replaceAll('\r\n', '\n'), normalized, file);
  }
}
console.log(JSON.stringify({ sourceSha256: sha(source), hooks: hooks.length, sections: inventory.sections.length, advancedCalls: calls }));
