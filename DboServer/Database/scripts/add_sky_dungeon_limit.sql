-- =====================================================================
-- add_sky_dungeon_limit.sql
--
-- PURPOSE:
--   Adds the sky_dungeon_limit table (daily Sky Dungeon entry count) to an
--   EXISTING dbo_char database. Fresh databases created from dbo_char.sql
--   already have it.
--
-- USAGE:
--   mysql -u root -p dbo_char < add_sky_dungeon_limit.sql
-- =====================================================================

CREATE TABLE IF NOT EXISTS `sky_dungeon_limit` (
  `char_id` int(10) unsigned NOT NULL,
  `enter_count` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `enter_day` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'YYYYMMDD of the daily period (resets at 04:00) enter_count belongs to',
  PRIMARY KEY (`char_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
