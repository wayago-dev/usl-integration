# Changelog

## v1.7.1

- GitHub Actions now packages one combined release for Windows, macOS, iOS and Android on version tags.
- Keeps the `ultimateshittylist.fun` endpoint and in-game automatic updater introduced in v1.7.0.

## v1.7.0

- Moved the default API and website links to ultimateshittylist.fun
- Added an in-game update banner that downloads the latest GitHub release and installs it after the game is restarted

## v1.6.4

- Search bar now copied verbatim from IntegratedDemonlist: CCMenu at the top of the list, explicit input/button positions, no layout container - sits flush under the title bar like the reference mod

## v1.6.3

- Fixed search bar position: it was overlapping the list title bar and got clipped. Now it sits fully visible between the title bar and the levels (background unchanged)

## v1.6.2

- Search bar now matches Global-List Integration exactly: tan background strip, row-layout container, scaled input and search button

## v1.6.1

- Removed the blue background strip from the search bar - now just the GD-style input and search button
- Fixed the search button getting stuck in the pressed state (search bar no longer hides during loading)
- Rank badge icons now have a white outline and drop shadow like GD level icons

## v1.6.0

- List page rebuilt on the native GD list layout (GJListLayer) like Global-List Integration - the level cells finally render inside the list again
- Search bar strip (USL blue) inside the list below the title bar, like Global-List but in USL style
- Rank badge on the level info page moved from the title corner into the stats column, icon reduced to 23px (Global-List style)

## v1.5.0

- Search bar added to the list page (USL blue strip with text input and search button, like Global-List Integration but in USL style)
- Fixed the level list rendering outside the panel (anchor/position mismatch)
- Search filters levels by name locally, Enter key works too
- "No results found" message when a search matches nothing

## v1.4.1

- Rank badge on level info page: added the round USL icon to the right of the #N number
- Rank badge in level cells: moved the round icon to the right of the #N number

## v1.4.0

- Fixed broken list panel: the old texture (GJ_panel_02) does not exist in GD 2.2, the panel rendered as a broken magenta/black square. Now uses the native GD 2.2 dark blue panel (GJ_square02) via nine-slice

## v1.3.0

- Removed the default green title bar from the list page - replaced with a custom panel header: round USL logo, title and page indicator in one clean row
- Button icon now fills the whole button (logo scaled up to the circle edge, no white ring)
- Fixed rank badge in level cells: it was overlapping the orbs stats, now placed after them; square icon replaced with the round logo

## v1.2.0

- Round USL logo icon in the menu buttons (white-ring circle badge instead of a square tile)
- Menu buttons resized to match the surrounding buttons
- List page cleanup: removed the floating logo and the floating page-number rectangle
- Page indicator now lives inside the list title bar (still clickable for jump-to-page)
- First/last page arrows moved to the bottom corners

## v1.1.0

- Fixed oversized USL button on the main menu and level search screen (small circle button, no more giant logo)
- List page now uses the USL site background image with a dim overlay
- Small USL logo mark next to the back button on the list page
- Rank badges (#N) shown next to levels in the USL list, in level search results and on the level info page
- Rank badges are gold and match the USL branding

## v1.0.0

- Initial release
- USL button in the level search screen and main menu
- USL list layer with pagination and real GD level cells
- Levels loaded from the USL API, cached between sessions
- Settings: API URL, levels per page, button toggles
