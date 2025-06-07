# structure

**modules/**
- contains settings and assets of all modules, including default module
- index.json: list of all available modules

**modules/[module name]**
- component settings (json): assets, items, characters, fonts, item descriptions, palettes, sounds
- assets: PNG images, TTF fonts, WAV sound files
- content: section texts (main + individual sections), maps, battle settings

# Example Module: default

## Entry in modules/index.json

```json
{
    "default": "DEFAULT",
    "modules": [
        {
            "id": "DEFAULT",
            "title": "THE BATTLEPITS OF KRARTH",
            "settings": "modules/default/settings.json"
        }
    ]
}
```

## Component settings (json files)

- **Main settings file** modules/default/settings.json

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
        "characters": "modules/default/characters.json"
    }
}
```

- **Image assets** modules/default/assets.json
- **Characters** modules/default/characters.json
- **Fonts** modules/default/font-settings.json
- **Item descriptions** modules/default/item-descriptions.json
- **Palettes** modules/default/palettes.json
- **Sounds** modules/default/sounds.json

## Assets / Resources

Because it is the default module, all PNG images, WAV sound files, and Content are in these directories under root (src/):

- **Battle definitions** modules/default/battles/*
- **Battle maps** modules/default/maps/*
- **Adventure specific assets** modules/default/images/*
- **Section texts**  modules/default/sections/*
- **TTF font files** fonts/*
- **PNG images (64 x 64)** images/*


