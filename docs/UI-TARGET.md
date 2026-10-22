# Quo Vadis¿ interface requirements

This is the current visual and interaction target. Quo Vadis¿ has its own panel,
title, controls and display presentation. Earlier hardware-recreation targets
are retired. Keep the sequencing engine, parameter/port IDs, patch compatibility
and editing workflows intact while refining the interface.

## Panel and layout

- Preserve the 34 HP footprint and all existing controls and connections.
- Use a classic DJ-mixer finish: charcoal over brushed silver by default,
  with an all-silver alternative. Add fine black rules, small red markers and
  restrained blue-grey heading strips. Both finishes are free of Roman ornament.
- Use **Quo Vadis¿** in clear sans-serif lettering. Keep manufacturer logos and
  author credits off the panel.
- Keep the eight pitch knobs, pulse faders, gate buttons and lower function
  buttons on the same eight centre lines. Gate and function buttons share exact
  left/right bounds. Native geometry defines alignment.
- Use plain fader slots with printed ticks and black caps with white index lines.
  Keep satin silver pitch knobs, black pointers and one cyan lamp per stage.
- Keep smooth round sockets. Outputs have contrasting filled label badges;
  inputs have plain labels. Reverse the badges on the all-silver plate.
- Keep the compact arrangement: the screen is 103 units high, pulse controls
  are 82 units high and the footer starts at y=286.

The [current native renders and dimensions](QUO-VADIS-UI.md) are the visual
reference. Use actual Rack captures to review changes at 100% and 200% zoom.
Do not use a flattened illustration as the interactive panel.

## Display and editing

Use the original two-track overview, scalable typography and coloured stage
diagrams. Settings pages retain their existing navigation and values, presented
with desktop text. Theme changes are available from the background context menu
and persist per patch without changing musical presets.

Every value-bearing control should support consistent upward/increasing and
downward/decreasing dragging, Shift fine adjustment, appropriate direct-value
menus and concise hover help. Preserve short-click actions when no drag occurs.
Labels and menus must identify the affected track, stage or MOD lane.

Changes made through controls, menus and optional editors must update the same
underlying settings, support undo/redo and survive saving/loading. Retain the
optional stage/MOD editors and keyboard performance view. Keep normal Rack cable,
port and context-menu behavior.

## Review

Run the targeted panel checks for layout changes and inspect native captures.
Verify exact column alignment, non-overlapping targets, all 31 buttons and 24
stage controls, style switching and patch save/reopen. Changes to value editing
also need the relevant gesture/undo checks. Actual mouse/keyboard playtesting
remains separate from automated callbacks.

Historic bitmap/reference headers remain internally for regression coverage;
they do not define the new panel's appearance. Removing those dependencies is a
separate refactor, not part of routine visual changes.
