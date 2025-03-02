# Blood Sword Redux
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

An SDL implementation of the revised Blood Sword gamebook (2014)

# Planned Features

- Moddable/Configurable (**IN PROGRESS**)
- New texture rendering and animation engine (**DONE**)
- Keyboard friendly with mouse and gamepad support (**DONE**)
- Fully digitized gameplay (**IN PROGRESS**)
- Quality of life improvements to record keeping (**IN PROGRESS**)
- Load / Save game at any point (**TO DO**)
- Sound (**TO DO**)
- Multiplatform Linux/Windows/OSX (**PARTIAL**)

## What happened to the previous attempt?

There has been a previous [attempt](https://www.github.com/daelsepara/blood-sword) to implement Blood Sword. Unfortunately, several technical roadblocks were encountered. Due to poor early design decisions, further features can only be implemented at great difficulty and increased code entropy.

With a view to making the game highly configurable as well as to ease the integration of the other books in the series, it has been decided to scrap the current effort and work towards a "better" engine from the ground up. Hopefully, severe breaking changes will be minimal as more books are integrated.

Futhermore, because of the improved system in the revised edition of the gamebook, to continue working with the old engine meant that inherent bugs from the early editions may be carried over into the code base.

## Will it still be completed?

After the implementation of the first book is completed in the new engine, it is certainly possible to revisit the previous attempt. Improvements from the new engine can then be incorporated, creating a kind of **classic** edition of the game.

## Progress and Pace

A slower, more methodical pace is being adopted to bring the new engine to fruition. This allows for more opportunities to do unit tests in the core systems. As expected, due to numerous real life<sup>tm</sup> and third world concerns among the software engineers involved in the project, progress in this endeavour takes a lower priority.

## Tests

**Scroll**

![Scroll](/screenshots/scroll.png)

**Character Stats**

![Stats](/screenshots/stats.png)

**Pathfinding**

![Pathfinding](/screenshots/pathfinding.gif)

**Battle Order**

![Battle Order](/screenshots/battle-order.gif)

**Battle/Combat**

## Fight/Shoot

![Fight/Shoot](/screenshots/fight-shoot.png)

## Deal fight/shot damage

![Damage](/screenshots/damage.png)

## Combat Spells (Call to Mind/Cast) **IN PROGRESS**

![Call Spell to Mind](/screenshots/call-to-mind.png)

![Cast Spell](/screenshots/cast-spell.png)

## Story Mode

### Introduction

![Introduction](/screenshots/story-mode-1.png)

### Story / Section Screen

![Section](/screenshots/story-mode-2.png)

### Choices

![Choices](/screenshots/story-mode-choices.png)

### Section: 261 Fight with Magus Vyl

![Choices](/screenshots/story-mode-magus-vyl.png)


# Copyright Information
```
            Originally published 1987 by Knight Books
     This Edition published 2014 by Fabled Lands Publishing
                 an imprint of Fabled Lands LLP

                           [FLP logo]
                      www.sparkfurnace.com

  Text copyright (c) 1987, 2014 Dave Morris and Oliver Johnson

 Maps and illustrations copyright (c) 1987, 2014 Russ Nicholson

    Cover art by Sébastien Brunet with thanks to Mikaël Louys

Thanks to Tim Savin for playtesting and rules suggestions and to
              Matt Hill for logo and cover design

The rights of Dave Morris and Oliver Johnson to be identified as
     the authors of this work have been asserted by them in
  accordance with the Copyright, Designs and Patents Act 1988.

                   ISBN-13: 978-1-909905-16-0
                     ISBN-10: 1-909905-16-X
```

**NOTE: This computer game implementation is not official nor is it endorsed by the gamebook's creators**

# Bugs

Feel free to report any bugs, issues, and/or suggestions. Thanks!

# Other Gamebooks implemented with SDL

- [Blood Sword (Classic)](https://www.github.com/daelsepara/blood-sword)
- [Necklace of Skulls](https://www.github.com/daelsepara/sdl-skulls)
- [Heart of Ice](https://www.github.com/daelsepara/sdl-heart)
- [Down Among the Dead Men](https://www.github.com/daelsepara/sdl-dead)
- [Green Blood](https://www.github.com/daelsepara/sdl-green)
- [Once Upon a Time in Arabia](https://www.github.com/daelsepara/sdl-arabia)
- [The Coils of Hate](https://www.github.com/daelsepara/sdl-coils)
- [Legendary Kingdoms: Valley of Bones](https://www.github.com/daelsepara/legendary-kingdoms)