# Blood Sword Redux : Documentation
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

The _Battlepits of Krarth_ gamebook content is w entirely through json files. These files are parsed and get processed futher by other internal systems that implement the gamebook's gameplay mechanics and visual presentation.

In this section, we document the format and functions of these json files for anyone interested in how _Battlepits of Krarth_ was implemented. This documentation is also intended for people interested in creating creating new content for the _BloodSword_ gamebook engine.

## JSON files

The BloodSword gamebook engine uses many different types of json files, which are documented in its own section. Sometimes these json files refer to other json files.

## [Settings](#settings)

This file contains information about the current module as well as references to other json files that drive the game's other core systems (help, story, starting ranks, item defaults).

- **implementation** [settings.json](../src/modules/default/settings.json)
- **documentation** [settings](settings/settings.md)

## [Book](#book)

- **implementation** [book.json](../src/modules/default/book.json)
- **documentation** [settings](book/book.md)

## [Assets](#assets)

List of icons used in the game, e.g. button icons, map sprites.

- **implementation** [assets.json](../src/modules/default/assets.json)
- **documentation** [assets](assets/assets.md)
