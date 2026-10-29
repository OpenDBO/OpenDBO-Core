-- =====================================================================
-- add_dwc.sql
--
-- PURPOSE:
--   Adds the Dragon World Challenge tables to an EXISTING dbo_char
--   database: dwc_limit (daily DWC entry count) and dwc_scenario (card
--   grade and clear count per mission). Fresh databases created from
--   dbo_char.sql already have them.
--
-- USAGE:
--   mysql -u root -p dbo_char < add_dwc.sql
-- =====================================================================

CREATE TABLE IF NOT EXISTS `dwc_limit` (
  `char_id` int(10) unsigned NOT NULL,
  `enter_count` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `enter_day` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'YYYYMMDD of the daily period (resets at 04:00) enter_count belongs to',
  PRIMARY KEY (`char_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `dwc_scenario` (
  `char_id` int(10) unsigned NOT NULL,
  `dwc_tblidx` int(10) unsigned NOT NULL,
  `mission_index` tinyint(3) unsigned NOT NULL COMMENT 'index into sDWC_TBLDAT::aMissionTblidx',
  `reward_grade` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT 'highest card grade obtained, 255 = none',
  `clear_count` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`,`dwc_tblidx`,`mission_index`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
