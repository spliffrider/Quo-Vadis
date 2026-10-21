# Quo Vadis¿ panel preview — 2.72.4

Quo Vadis¿ has two original vector panels: Classic metal and Atrium.
The plugin draws scalable artwork and separate interactive controls in Rack.
The native captures linked below show the current implementation.

Classic metal uses the finish approved on 2026-10-21: neutral brushed
aluminium on the body and footer, charcoal above, satin silver controls and
small burgundy/blue-grey printed heading accents. Version 2.72.3 corrects the
earlier pale blue-grey cast and makes the horizontal brush marks visible.
The silver and charcoal use small deterministic material textures generated
locally; no reference-image pixels are embedded. Textures are created once per
NanoVG context, released on context teardown, and cached with the panel art.
Moving controls remain separate interactive widgets.
The finish does not change sequencing or patch identifiers.
Atrium retains its warm stone palette and ornament.

Version 2.72.4 removes the broad empty band above the footer in both styles.
The footer divider moves from y=310 to y=286; pulse columns are 82 units tall
instead of 106, and the screen is 103 instead of 111. Navigation, transport,
CTRL and function controls use larger targets within the same Rack footprint.
The overview and stage graphs reflow vertically; overview text sizes and pulse
drag sensitivity are retained. All eight horizontal stage positions stay fixed.

Right-click the module background → **Panel style → Classic metal / Atrium**.
The style changes immediately and is stored in the Rack patch. It is not a
sequencer parameter and musical preset changes do not replace it. Legacy
patches and invalid style values choose Classic metal. Settings backups accept
the optional style field; backups without it remain valid.

Both styles retain the same 34 HP footprint, port IDs, 789 parameter IDs,
31 pushbuttons, encoder, AUX/CTRL knobs and 24 stage controls. Pitch now uses
satin silver rotaries with a black pointer and one cyan activity lamp per
stage. Pulse faders remain aligned with the pitch knobs and gate cells.
Title laurels sit outside the measured title bounds, with a gap of one title
font size. Atrium gate numbers have a separate row above the arches. There
is no foliage beside VALUE / ENTER.

Drag up to increase a value, down to decrease. Shift gives fine movement;
right-click exposes numeric entry and context choices. Existing undo/redo,
hover help, active adjustment tips, button gestures, shortcuts and the desktop
performance view are retained. The cyan stage lenses use Rack's native light
layer and Bloom setting. Output labels have pale filled badges; input labels
sit directly on the black panel. Socket washers and bores use smooth concentric
circles, with continuous metal shading and no polygonal nut or segmented rim.

Gate-mode buttons and the lower stage/function buttons share one geometry
definition and one cap renderer: all eight pairs have exactly the same left
edge, centre line and right edge. Their common width is 26 Rack pixels, with
27.5 pixels between centres. The pulse/gate banks are narrowed from the left,
with stage 8 anchored at x=482.5 and the common right edge at x=495.5. Pitch,
pulse and gate controls use the lower function bank's compact spacing. The
widget check compares actual button bounds
without a tolerance. Direct Rack-rendered close-ups are the layout reference:
[Classic metal](../design/previews/2026-10-21/Classic-compact-stage-rows.png) and
[Atrium](../design/previews/2026-10-21/Atrium-compact-stage-rows.png).
Native widget coordinates define the exact runtime geometry.

Full native renders: [Classic metal](../design/previews/2026-10-21/Classic-compact-Rack.png)
and [Atrium](../design/previews/2026-10-21/Atrium-compact-Rack.png).

The larger display has an original two-track overview, scalable text and
coloured stage diagrams. Settings pages retain their underlying navigation
and values, rendered with desktop typography instead of the former OLED
bitmap artwork. Further visual tuning of individual pages is still expected.

The module-browser name and both panel titles are Quo Vadis¿, with the
inverted question mark at the end. Cinzel provides the glyph directly;
title size and measured laurel spacing are retained. Legacy plugin/model slugs and backup
type identifiers remain for file compatibility. The visible panel uses original
drawing/material code and the Cinzel font with its OFL license; previous logos
and hardware-panel reference photographs are excluded from the new repository
and packages. Historical bitmap/reference headers remain internally for
navigation and regression tests. Their removal requires a separate refactor.

Validation:

- Prior integration coverage (`scripts/test-rack.ps1`): parameter/preset/chain/clock/MIDI/MX integration,
  settings backup migration and new style roundtrip/invalid/legacy cases.
- `scripts/test-panel-styles.ps1`: actual Rack widget callbacks for all 24 stage
  controls, alignment, all 31 buttons, non-overlapping hit targets, actual style-menu actions, screenshots
  at 100% and 200%, and a real `.vcv` save/reopen retaining Atrium.
- Native screenshots are generated under `tmp/panel-style-qa/`.
- Prior DSP validation (`scripts/test-production.ps1`): the stripped 2.72.1 Windows package passed 361
  virtual seconds at 44.1/48/96 kHz, 22,624,609 processed frames, 145 in-place
  restores and 144 fresh instances, with exact state restoration.

The callback tests do not replace hands-on mouse/keyboard testing or hardware
comparison. The repository and
[Windows 2.72.4 release](https://github.com/spliffrider/Qua-Vadis/releases/tag/v2.72.4)
remain private.
