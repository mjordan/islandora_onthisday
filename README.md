# Islandora On This Day

Utility module that queries Solr for objects whose date fields contain a month and day equal to the current day's. The module displays a gallery of thumbnails at `/onthisday` for the objects it finds in Solr:

![Sample On This Day gallery](images/onthisday_gallery.png)

It also:

* provides a block that displays a random object from the list of those whose month and day match the current day's
* provides an RSS feed at `/onthisday/rss` containing items corresponding to the current day's gallery
* provides a JSON version of the entries in the current day's gallery at `/onthisday/json`, suitable for use in timeline applications, etc.

The module lets admins configure a list of date query and sort fields in Solr, but assumes that date values are in YYYY-MM-DD format (which is a recommended format for dates in MODS, for example).

## Requirements

* [Islandora](https://github.com/Islandora/islandora)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search)

## Usage

Enable this module as you would any other, and configure it at `admin/islandora/tools/onthisday`. Once you do this, visiting `/onthisday` will display a list of objects indexed in Solr that have date information containing the current month and day; the year component of the date will be displayed at the end of each object's title.

This module also provides an "Islandora On This Day" block that will show one randomly selected object that has the current month/day. The block will not appear if there are no matching objects. The block has its own options for showing collection links and a "See more" link to the gallery.

Admins of very large Islandora sites will want to consider using the included Drush script to generate and clear data cached by this module.

## Testing your gallery

You can test your gallery by appending a test month and day to the end of `/onthisday`. For example, `/onthisday/12-31` will show all objects with dates ending in 12-31 (1945-12-31, 2003-12-31, etc). For this to work, you need to check the "Enable adding test dates in URL" setting in the module's admin options. Make sure this option is turned off in production. Also note that this feature only works while caching of "on this day" items is turned off.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

 [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
