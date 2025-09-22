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

## [assets](#assets)

JSON file which lists the sprites/icons used in the game. Path to file must be relative to the game executable's location, usually inside the module's directory. For the **default** module, this file (**assets.json** in the above example) is inside the **default** module's directory at */modules/default*.

**see** [assets](../assets/assets.md)

## [fonts](#fonts)

JSON file which determines the font file used in the game as well as sizes for *normal*, *fixed* and *caption* text. It is inside the **default** module's directory at */modules/default*.

**see** [fonts](../fonts/fonts.md)

[go back](../README.md#settings)
