# LoadingScreenTweaks Changelog
## v1.7.0
- Added `Loading Progress Percentage Text` and `Load Step Progress Text`.
  - `Load Step Progress Text` is definitely the more accurate of these two options.
  - You can still use both options together if you'd like, but there are no promises that `Loading Progress Percentage Text` is accurate.
- Fix a long-standing bug where the default set of splash texts could not be toggled off.
  - Incidentally, also fix a bug where a quote from the default set of splash texts was twice as likely to appear.
  - (Darn, how did I screw that up?)
## v1.6.2
- Experimental iOS support.
## v1.6.1
- Refactor shuffling algorithms because that's apparently what people do in their free time, I guess.
## v1.6.0
- Added a "refresh quotes" button for convenience. No more full game restarts!
    - Toggling on/off quotes while the game is open will auto-reload quotes for the mod to choose from.
- Allow toggling on/off the default set of quotes.
- Added 50 quotes from SNL to celebrate SNL50.
    - "Live from New York, it's Saturday Night!"
- Fix some bugs here and there.
## v1.5.1
- Fix a stupid C++ crash for shuffling empty data types.
## v1.5.0
- Add some long-overdue `nullptr` checks.
## v1.4.2
- Port to 2.2074. Shoutout to [B1rtek](https://github.com/b1rtek) and [Fleeym](https://github.com/fleeym) for helping me deal with notoriously clunky SettingsV3 syntax!
## v1.4.1
- Added "Custom Quotes Only" option, by popular demand. (If no custom quotes are found, the mod will fall back to anything enabled from the default set of quotes.)
- Added Futurama quotes, because why not. (Disabled by default for your sanity.)
- Skip to v1.4.1 for consistency with DeathScreenTweaks.
## v1.3.1
- Fix a crash from selecting Fonts 1 through 9 (even though they're all objectively ugly). Thanks again, hiimjustin000!
## v1.3.0
- Port to 2.206. Big thanks to CatXus, Aktimoose, ninXout, and hiimjustin000 for helping out with testing!
- Added font setting for splash text.
## v1.2.0
- Allow users to customize the text area on the loading screen, much like DisturbingNewBests and WholesomeEndTexts.
## v1.1.0
- Updated to use Node IDs instead of grabbing by index.
## v1.0.0
- Initial release.
