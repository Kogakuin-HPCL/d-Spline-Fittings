# Release Note

[English](CHANGELOG.md) | [日本語](CHANGELOG-ja.md)

## Version 0.1.2+docs (2025-03-09)

- Other Updates
  - Added the English version of the README
- Known Issues
  - Using the two-phase model, there are some cases that the utilities output results that seems a bit off to the human eye (2025-03-09: under investigation)

## Version 0.1.2 (2025-03-07)

- Improvements
  - Improved builds to be completed only with `make` in the root directory
  - Changed specification so that options can be omitted when using the tool
- Bug Fixes
  - Fixed a bug that required wrong options when passing x and y arrays as separate files when using the tool
- Other Updates
  - Unified the text displayed when using the tool in English
  - Removed the `datafile` directory
- Known Issues
  - Using the two-phase model, there are some cases that the utilities output results that seems a bit off to the human eye (2025-03-09: under investigation)

## Version 0.1.1 (2025-03-05)

- Improvements
  - Changed specification to allow compilation of user programs outside this library directory
  - Changed the type of generated libraries to shared libraries, so that the `-lm` option, which was required when using libraries, is no longer necessary
  - Added specification to automatically generate a script that adds the path to the generated libraries to the environment variables
- Bug Fixes
  - Fixed a problem that the directory `lib` needed for library generation was not registered in the Git repository

## Version 0.1.0+changelog (2025-02-27)

- Others
  - Added the Release Note (in Japanese only)

## Version 0.1.0 (2025-02-25)

Initial release.
