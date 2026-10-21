# Quo Vadis¿ interface requirements

This is the current visual and interaction target. Quo Vadis¿ has its own panel,
title, controls and display presentation. Earlier hardware-recreation targets
are retired. Keep the sequencing engine, parameter/port IDs, patch compatibility
and editing workflows intact while refining the interface.

## Panel and layout

- Preserve the 34 HP footprint and all existing controls and connections.
- Classic metal uses a charcoal upper section, neutral brushed-silver body and
  footer, satin silver pitch knobs with black pointers, and restrained burgundy
  and blue-grey printed accents. Atrium remains an optional warm-stone style.
- Use the title **Quo Vadis¿**, Cinzel lettering and widely spaced laurels.
  Keep manufacturer logos and author credits off the panel.
- Keep the eight pitch knobs, pulse faders, gate buttons and lower function
  buttons on the same eight centre lines. Gate and function buttons share exact
  left/right bounds. Native geometry defines alignment.
- Use Doric pulse surrounds, one cyan lamp beneath each pitch knob, smooth round
  sockets and light output-label badges. Inputs use plain labels on charcoal.
- Keep the compact 2.72.4 arrangement: the screen and pulse bank are shorter,
  the footer starts at y=286, and lower controls fill the former empty band.
- Give Atrium gate numbers their own clear row above the arches; omit ornament
  beside VALUE / ENTER. Keep ornament clear of labels and hit targets.

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
