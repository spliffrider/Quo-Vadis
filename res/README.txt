Quo Vadis¿ runtime assets

Cinzel.ttf: Cinzel by The Cinzel Project Authors.
Upstream: https://github.com/NDISCOVER/Cinzel
Copyright 2020 The Cinzel Project Authors.
Licensed under the SIL Open Font License 1.1, included as Cinzel-OFL.txt.

The visible panel, controls, ornament and display presentation are drawn in C++.
Classic's brushed-silver and charcoal textures are generated procedurally; no
reference-photo pixels are embedded in those textures. Display typography also
uses Rack's system font through the host API.

Retired logos and panel reference photographs are not runtime assets. Historical
bitmap/reference headers still support internal regression tests and navigation
logic; their removal would require a separate source/test refactor.
