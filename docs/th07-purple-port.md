# TH07 purple THPrac migration — in progress

This is a partial isolated implementation, NOT a completed migration or a
playable Runtime release. The canonical `th07` checkout, Launcher and deployed
sites are unchanged.

## Source and ownership

- Canonical Eagler base: `dccaf4e` (`eagler`).
- Experiment: `_scratch/th07-thprac-purple`, `experiment/th07-thprac-purple`.
- Purple authority: `thprac/thprac_purple`, commit
  `17780056107a014e88d35fceb3ad86c121d94287`.
- Normalized TH07 native source SHA-256:
  `b5be6dc698f574992ab49696c5c0d8ad756af93bf0760be1504ee9e958bee1b0`.
- Development authority: Eagler `ADAPTING_A_GAME.md`, THPrac playbook and
  adaptation-worktree isolation playbook.
- Generated inventory identifies 103 section names and 43 top-level named
  EHOOK/PATCH sites. It is NOT proof that the sections/hooks are implemented;
  macro-generated hotkey patch sites additionally require their own audit.

## Connected in this checkpoint

- Native `GuiHotKey::OnWidgetUpdate`: text, green active color, brackets,
  measured invisible-button bounds, clickable toggle, balanced color stack.
  The generator only substitutes formatting and removes the external locale
  lookup; original implementation supplies widget behavior.
- Backspace rows F1–F8 and U. F8 information shortcut retains the existing Tab
  alias. Browser code/key/keyCode forms reach the C++ F12/U/F8 bit owner.
- U follows native `mEnemyMuteki` patch `0x421447`: suppress only the enemy
  life subtraction. Retain computed damage, lastDamage, score/cherry/collision.
- Native disable-X/Z/Shift and force-Shift filter statements, including C's
  dependency on disabled X and force-Shift precedence. Filter the merged live
  gameplay sample, after keyboard/controller/touch merging and before recording.
  Preserve Replay and THPrac-OFF input. ImGui navigation owns separate raw input.
- Native no-continue infinite-life mapping (`th07_inf_lives`, `0x441171`)
  applies the pre-decrement `< 1` gate. Preserve the native F2 game-over bypass
  when the stock lives are zero; do not write a synthetic positive life count.
- Extracted purple MIT license is independent of the absent external GUI
  extension. Existing Practice configuration, section timings and Replay
  provenance remain the BLUE implementation at this checkpoint.

## Checks actually run

- Generator `--write`, then `--check`: PASS.
- Actual bundled ImGui regression: cold-open, measured click/release, on/off,
  twelve frames, draw-data and item/color-stack balance: PASS. This is NOT
  physical touch or browser-gameplay evidence.
- 2,097,152 input samples, OFF/Replay passthrough, unrelated-bit preservation,
  C dependency and Shift precedence: PASS.
- Browser hotkey fallback mapping and production-owner source boundaries:
  PASS (source evidence, not full-game execution).
- Emscripten syntax checks of modified production PracticeRuntime,
  ThpracImGui, EnemyManager, Controller and Player with THPrac enabled: PASS,
  without the missing portable extension. Existing format-security warning in
  BuildUpstreamReplayJson remains unchanged.
- Ordinary THPrac-OFF stub, Controller, EnemyManager and Player syntax checks:
  PASS. No generated game content is included.
- Full link/build, original demos, long Replay golden checks, browser/phone
  interaction and public deployment: NOT RUN.

## Remaining before promotion

- Recover/materialize the referenced `thprac-reallyportable/portable` source,
  or explicitly replace that extension with a fully audited purple extractor.
  Current CMake integration requires `TH_RUNTIME_EXTENSION_CMAKE` and
  `thprac_portable_attach`; the extension and `section_catalog.hpp` are absent
  from this workspace. The existing source audit additionally references
  missing `th07-eagler` paths; do not weaken those tests to claim completion.
- Verify all native practice fields, 103 sections, timeline/health/time/dialogue
  patch paths, Extra/Phantasm lifecycle, and blue replay compatibility.
- Connect and verify the remaining native F12 owners: key HUD/APS, reaction
  test, fast retry, game speed (logical cadence, not Launcher high refresh),
  lock-timer display, projectile/laser hitbox display, one-key death and
  always-save-score/spell-history. Do not import Launcher-only tools.
- Audit information/tracker counters and native save/read/checksum/MIDI hooks.
- Materialize pinned build dependencies, build a source-only Runtime and run
  lifecycle, replay determinism, enabled/OFF and mobile acceptance gates.
- Review and promote the complete experiment diff into canonical Eagler only
  when its owners and evidence are closed. No deployment or push is authorized
  merely by this checkpoint.

Run the current isolated gates with explicit source/tool roots:

```text
node scripts/generate-purple-thprac.mjs <purple-checkout> --check
EMSDK=<local-sdk> THPRAC_IMGUI_ROOT=<reviewed-imgui> node tests/test-purple-menu.mjs
```
