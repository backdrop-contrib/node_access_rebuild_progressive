# Node Access Rebuild Progressive

Provides an alternative means of rebuilding the Content Access table.

## Rationale

The default core behaviour, when a module marks the content_access table for rebuilding, is to delete all entries first and to then rebuild the grants for all nodes.

This conservative approach prevents anyone from accessing a node they should not have access to according to the new rules, until all acces rules have been computed. It bends towards a secure "deny first" approach, but this has a major drawback.
By preventing everyone to access everything at first, it means the whole site is basically unusable during the whole operation. For sites with a large number of nodes and/or lots of complex hook_node_grants implementations, it can be very lenghty and results in a lot of downtime.

The trade off implemented here consists in leaving the "old" access rules in place for a given node until the new ones have been computed.

This means someone could potentially still access some content they should no longer have the rights to until the new rules are in place. But it also means the site can continue to operate normally while the access rebuild takes place in the background.

## Approach

The module works by processing nodes in chunks, from highest node.nid to lowest, until all nodes have been recomputed, storing the last processed nid as state. This ensure no nodes can be skipped, while avoiding having to populate a Queue or other equivalent mechanism, which can be heavy in itself.

This can be processed either at cron run, one "chunk" at each cron call, or more efficiently, all at once through the provided drush command.

## Usage

While you can configure the module to run at cron time, the preferred method is to use drush on demand:

`drush node-access-rebuild-progressive`

## Installation

Install this module using the official Backdrop CMS instructions at
<https://backdropcms.org/guide/modules>.

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

## Maintainers

* Seeking co-maintainers.

## Credit

Ported to Backdrop by [herbdool](https://github.com/herbdool).

Drupal maintainers:

* <https://www.drupal.org/u/dioni>
* <https://www.drupal.org/u/bellesmanieres>
* <https://www.drupal.org/u/shelane>
