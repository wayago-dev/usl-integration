# USL Integration

[![Geode](https://img.shields.io/badge/Geode-5.8.2-5967c9)](https://geode-sdk.org)
[![GD](https://img.shields.io/badge/GD-2.2081-00aff0)](https://geometrydash.com)

Browse the official **Ultimate Shitty List** demon ranking right inside Geometry Dash and play the levels with one click.

## Features

- **USL button** on the level search screen and in the main menu
- **Live data** - the list is fetched from the [USL API](https://ultimateshittylist.space/api/levels), so new placements appear as soon as they are published
- **Real GD levels** - every row is the actual level from the Geometry Dash servers, click it to play
- **Pagination** - first, previous, next, last page and jump-to-page
- **Refresh** button to pull the latest list at any time
- **Settings**: API URL, levels per page, which buttons to show

## Installation

1. Install [Geode](https://geode-sdk.org) for Geometry Dash 2.2081
2. Download the mod from the [releases page](https://github.com/wayago-dev/usl-integration/releases)
3. Drop the `.geode` file into your mods folder, or open it with Geode

## Usage

Click the USL logo button in the level search screen (top right) or in the main menu to open the list. Use the arrow buttons or the left/right arrow keys to flip pages. Click any level to play it.

## Building

Requires the Geode SDK 5.8.2+ and CLI, with `GEODE_SDK` set.

```powershell
cmake -B build
cmake --build build --config Release
```

## Links

- [Ultimate Shitty List](https://ultimateshittylist.space)
- [USL API docs](https://ultimateshittylist.space/api/docs)

## License

MIT
