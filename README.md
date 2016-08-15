# Islandora On This Day

Utility module that queries Solr for objects whose date fields contain a month and day equal to the current day's. It displays a gallery of thumbnails for the objects it finds at /onthisday.

Still under development.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)

## Usage

Enable this module as you would any other, and configure it at admin/islandora/tools/onthisday.

You can add a test month and day to the end of /onthisday for testing purposes. For example, /onthisday/12-31 will show all objects with dates ending in 12-31 (2000-12-31, 2003-12-31, etc). For this to work, you need to check the "Enable adding test dates in URL" setting in the module's admin options.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## Planned Development

* Cache results
* Provide a block that shows a random object with today's date

## License

 [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
