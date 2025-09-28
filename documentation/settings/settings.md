# Settings

```json
{
    "settings": {
        "assets": "modules/default/assets.json",
        "fonts": "modules/default/font-settings.json",
        "sounds": "modules/default/sounds.json",
        "palettes": "modules/default/palettes.json",
        "palette": 0,
        "items": "modules/default/items.json",
        "item_descriptions": "modules/default/item-descriptions.json",
        "adventure": "modules/default/book.json",
        "characters": "modules/default/characters.json",
        "ranks": "modules/default/ranks.json",
        "about": "Originally published 1987 by Knight Books.\n\nThis Edition published 2014 by Fabled Lands Publishing an imprint of Fabled Lands LLP.\n\nwww.sparkfurnace.com\n\nText copyright (c) 1987, 2014 Dave Morris and Oliver Johnson.\n\nMaps and illustrations copyright (c) 1987, 2014 Russ Nicholson.\n\nCover art by Sébastien Brunet with thanks to Mikaël Louys.\n\nThanks to Tim Savin for playtesting and rules suggestions and to Matt Hill for logo and cover design.\n\nThe rights of Dave Morris and Oliver Johnson to be identified as the authors of this work have been asserted by them in accordance with the Copyright, Designs and Patents Act 1988.\n\nISBN-13: 978-1-909905-16-0\nISBN-10: 1-909905-16-X\n\nDigital Adaptation by: SD Separa (2025)\nIcons: https://game-icons.net\nJSON Library: https://github.com/nlohmann/json\n\n",
        "logo": "images/logo/bloodsword.png",
        "help": "help/help.json"
    }
}
```

This is the main json file from which other json files are loaded and parsed by the engine. It is the first json file parsed.

**see** [settings.json](../../src/modules/default/settings.json)

## Format

Path to file(s) must be relative to the game executable's location. The files are usually inside the module's directory, e.g. for the default module, they are inside the **default** module's directory at */modules/default*.

## [assets](#assets)

JSON file which lists the sprites/icons used in the game.

**see** [assets](../assets/assets.md)

## [fonts](#fonts)

JSON file which determines the font file used in the game as well as sizes for *normal*, *fixed* and *caption* text.

**see** [fonts](../fonts/fonts.md)

## [sounds](#sounds)

JSON file which lists the sound effects used for in game events, e.g. **DICE ROLL**, **BUTTON CLICK**, etc.

**see** [sounds](../sounds/sounds.md)

## [palettes](#palettes)

JSON file which lists available 4-color palettes. Each palette in the list contains color definitions for **NORMAL**, **HIGHLIGHT**, **INACTIVE**, and **BACKGROUND**. This list is used with the [**palette**](#palette) setting below.

**see** [palettes](../palettes/palettes.md)

## [palette](#palette)

Index to the current color palette used. Default is *0*. Used with the [**palettes**](#palettes) list above.

**see** [palettes](../palettes/palettes.md)

## [items](#items)

JSON file for the default definitions for common items found in the current module.

**see** [items](../items/items.md)

## [item_descriptions](#item-descriptions)

JSON file for the default descriptions for common items found in the current module.

**see** [item descriptions](../items/descriptions.md)

## [adventure](#adventure)

JSON file for the current module's gamebook. It contains the current module/book's title, description, as well as a list to all of the book's sections.

**see** [adventure](../adventure/adventure.md)

## [characters](#characters)

JSON file for the extra characters in the current module, i.e. player-controllable characters not included in the default characters (**WARRIOR**, **TRICKSTER**, **SAGE**, **ENCHANTER**).

**see** [characters](../characters/characters.md)

## [ranks](#ranks)

Starting character ranks based on the size of the party, e.g. Character ranks start at 8 if there are is only one (1) in the party, rank 4 for a two (2) character party, 3 for a three (3) character party, and 2 for a full party of four (4).

**see** [ranks](../ranks/ranks.md)

## [about](#about)

Miscellaneous information about the current module. This section is usually for copyright and trademark information.

[go back](../README.md#settings)
