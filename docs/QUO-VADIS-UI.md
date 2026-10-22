# Quo Vadis¿ panel preview — 2.72.5

The native panel now uses a restrained DJ-mixer finish: brushed aluminium,
black sections, fine printed graduations and black fader caps with white index
lines. Roman columns, arches, laurels, mosaic borders and inscription lettering
have been removed. The name remains **Quo Vadis¿**, now in sans-serif type.

Right-click the module background → **Panel style → Black / silver** or
**Brushed silver**. The default has a charcoal upper section and silver body;
the second has an entirely brushed-silver plate. Both retain the same controls.

The old numeric style IDs are preserved. Existing Classic patches open in
Black / silver; existing Atrium patches open in Brushed silver. The choice
survives saving/loading and is independent of musical presets. Patches without
a style field, or with an invalid value, use Black / silver.

## Native appearance and geometry

[Black / silver](../design/previews/2026-10-22/Black-silver-Rack.png) ·
[Brushed silver](../design/previews/2026-10-22/Brushed-silver-Rack.png)

These images are rendered from actual Rack widgets. The panel uses procedural
metal and charcoal textures, generated once per NanoVG context and cached with
static art. No pixels or logos from the mixer photographs are embedded.
The title uses Rack's bundled DejaVu Sans; the previous Cinzel asset is no longer
used or included in packages.

The 510 × 380 panel keeps the compact 2.72.4 geometry: 103-unit display,
82-unit pulse controls and footer at y=286. The eight stage centres are
290, 317.5, 345, 372.5, 400, 427.5, 455 and 482.5. Gate and function cells have
identical 26-unit widths and exact shared left/right edges. Fader index lines
and printed tick marks use the same value-to-position mapping.

All 31 labelled pushbuttons, 24 stage controls, encoder, AUX/CTRL knobs,
five inputs and seven outputs remain. Parameter IDs, port IDs, patch footprint,
sequencing and display navigation are unchanged. The silver pitch knobs retain
black pointers and one cyan lamp each. Output labels have contrasting filled
badges, inverted on the silver plate; inputs use plain labels. Jacks remain
smooth and circular. Native Rack Bloom controls the stage lamps.

[Exact alignment coordinates](../design/previews/2026-10-22/stage-button-alignment.csv) ·
[Black / silver button rows](../design/previews/2026-10-22/Black-silver-stage-rows.png) ·
[Brushed silver button rows](../design/previews/2026-10-22/Brushed-silver-stage-rows.png)

## Validation

`scripts/test-panel-styles.ps1` exercises actual Rack callbacks for all 24 stage
controls, counts the 31 labelled buttons, checks exact paired bounds and
non-overlapping hit targets, selects both named theme-menu entries, captures
100%/200% renders and saves/reopens a Rack patch in Brushed silver.
It counts labelled buttons rather than exercising every button action.

Style persistence checks in `tests/PanelStyleRackTests.hpp` cover both style
IDs, invalid/missing fields, migration of the former style ID 1 and independence
from musical preset recall. The [current evidence](../design/previews/2026-10-22/mixer-panel-evidence.json)
records the checks actually run for this build.

The redesign is validated on Windows Rack Free 2.6.6. Native Linux/macOS GUI
validation and broader hands-on playtesting remain separate. A known Linux
performance-keyboard QA failure from 2.72.4 remains under investigation;
this visual update does not claim to fix it. Physical MIDI-controller testing
has not been performed. Separate per-track scales remain unimplemented.

The repository and [Windows preview releases](https://github.com/spliffrider/Quo-Vadis/releases)
are public. Internal plugin/model/backup identifiers remain for compatibility.
