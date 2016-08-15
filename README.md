# Islandora On This Day

Utility module that queries Solr for objects whose date fields contain a month and day equal to the current day's. The module:

* displays a gallery of thumbnails at /onthisday for the objects it finds in Solr
* provides a block that displays a random object from the list of those whose month and day match the current day's.

The module lets admins configure a list of fields to query in Solr for date information, but assumes that the date values are in YYYY-MM-DD format (which is a recommended format for dates in MODS, for example).

## Requirements

* [Islandora](https://github.com/Islandora/islandora)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)

## Usage

Enable this module as you would any other, and configure it at `admin/islandora/tools/onthisday`. Onc you do this, you should be able to see objects indexed in Solr that have date information containing the current month and day; the year component of the date will be displayed at the end of each object's title.

You will also see a block titled "Islandora On This Day" that will show one randomly selected object that has the current month/day. It will not appear if there are no matching objects.

## Testing your gallery

You can test your gallery by appending a test month and day to the end of /onthisday. For example, /onthisday/12-31 will show all objects with dates ending in 12-31 (2000-12-31, 2003-12-31, etc). For this to work, you need to check the "Enable adding test dates in URL" setting in the module's admin options. Make sure this option is turned off in production.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## Planned Development

The module is still in relatively early development and testing. Here are some features that are planned:

* Provide an RSS feed of the day's objects.
* Provide a setting for maximum number of objects to display at /onthisdate; the subset should probably be a randomized subset of all records found in Solr as opposed to a limit on the number of documents retrieved from Solr.
* Provide a way for the admin to exclude specific collections and content models from the list of daily objects.

## License

 [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
