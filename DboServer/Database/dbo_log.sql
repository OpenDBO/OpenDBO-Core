/*
Navicat MySQL Data Transfer

Source Server         : db
Source Server Version : 80018
Source Host           : localhost:3306
Source Database       : dbo_log

Target Server Type    : MYSQL
Target Server Version : 80018
File Encoding         : 65001

Date: 2020-04-21 14:06:37
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for auctionhouse_log
-- ----------------------------
DROP TABLE IF EXISTS `auctionhouse_log`;
CREATE TABLE `auctionhouse_log` (
  `seller_char_id` int(15) unsigned DEFAULT NULL,
  `buyer_char_id` int(15) unsigned DEFAULT NULL,
  `price` int(15) unsigned DEFAULT NULL,
  `item_tblidx` int(15) unsigned DEFAULT NULL,
  `item_id` bigint(30) unsigned DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auth_login_log
-- ----------------------------
DROP TABLE IF EXISTS `auth_login_log`;
CREATE TABLE `auth_login_log` (
  `account_id` int(15) unsigned NOT NULL,
  `ip` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `account_id` (`account_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for budokai
-- ----------------------------
DROP TABLE IF EXISTS `budokai`;
CREATE TABLE `budokai` (
  `season_count` int(6) unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of budokais. 0 = 1, 10 = 11 ...',
  `default_open_time` int(15) unsigned DEFAULT '0',
  `rank_point_initialized` bit(1) DEFAULT b'0',
  `state_data_state` tinyint(3) unsigned DEFAULT '0' COMMENT 'Budokai State',
  `state_data_next_step_time` int(15) unsigned DEFAULT '0' COMMENT 'budokai next step time',
  `individual_state_data_state` tinyint(3) unsigned DEFAULT '0' COMMENT 'Solo match state',
  `individual_state_data_next_step_time` int(15) unsigned DEFAULT '0' COMMENT 'solo match next step time',
  `team_state_data_state` tinyint(3) unsigned DEFAULT '0' COMMENT 'team match state',
  `team_state_data_next_step_time` int(15) unsigned DEFAULT '0' COMMENT 'Team match next state time'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for budokai_winners
-- ----------------------------
DROP TABLE IF EXISTS `budokai_winners`;
CREATE TABLE `budokai_winners` (
  `budokai_number` int(10) unsigned DEFAULT '0',
  `type` tinyint(1) DEFAULT '0' COMMENT '0 = junior, 1 = adult',
  `match_type` tinyint(1) DEFAULT '0' COMMENT '0 = individual, 1 = team',
  `winner_char_id_1` int(15) unsigned DEFAULT '0',
  `winner_char_id_2` int(15) unsigned DEFAULT '0',
  `winner_char_id_3` int(15) unsigned DEFAULT '0',
  `winner_char_id_4` int(15) unsigned DEFAULT '0',
  `winner_char_id_5` int(15) unsigned DEFAULT '0',
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for change_char_name
-- ----------------------------
DROP TABLE IF EXISTS `change_char_name`;
CREATE TABLE `change_char_name` (
  `char_id` int(15) unsigned NOT NULL,
  `name` varchar(17) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `new_name` varchar(17) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for character_delete_log
-- ----------------------------
DROP TABLE IF EXISTS `character_delete_log`;
CREATE TABLE `character_delete_log` (
  `account_id` int(15) unsigned NOT NULL,
  `char_id` int(15) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for dynamic_field_count
-- ----------------------------
DROP TABLE IF EXISTS `dynamic_field_count`;
CREATE TABLE `dynamic_field_count` (
  `server_index` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`server_index`),
  UNIQUE KEY `server_index` (`server_index`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='When the server restarts it will save & load the progress from this table.\r\nSince it takes many days to reach max count it is very important.';

-- ----------------------------
-- Records of dynamic_field_count
-- ----------------------------
INSERT INTO `dynamic_field_count` (`server_index`, `count`) VALUES (0, 0);

-- ----------------------------
-- Table structure for founder_log
-- ----------------------------
DROP TABLE IF EXISTS `founder_log`;
CREATE TABLE `founder_log` (
  `username` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `forumname` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `date` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for gm_log
-- ----------------------------
DROP TABLE IF EXISTS `gm_log`;
CREATE TABLE `gm_log` (
  `char_id` int(15) unsigned NOT NULL,
  `log_type` int(3) unsigned DEFAULT NULL,
  `string` text CHARACTER SET utf8 COLLATE utf8_general_ci,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for guild_name_change_log
-- ----------------------------
DROP TABLE IF EXISTS `guild_name_change_log`;
CREATE TABLE `guild_name_change_log` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guild_id` int(15) unsigned DEFAULT NULL,
  `current_name` varchar(25) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `new_name` varchar(25) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for item_upgrade_log
-- ----------------------------
DROP TABLE IF EXISTS `item_upgrade_log`;
CREATE TABLE `item_upgrade_log` (
  `char_id` int(15) unsigned NOT NULL,
  `is_success` bit(1) NOT NULL,
  `item_id` bigint(20) unsigned NOT NULL,
  `item_tblidx` int(15) unsigned NOT NULL,
  `grade` int(3) NOT NULL,
  `new_grade` int(3) NOT NULL,
  `stone_item_id` bigint(20) unsigned NOT NULL,
  `stone_item_tblidx` int(15) unsigned NOT NULL,
  `core_item_use` bit(1) NOT NULL,
  `core_item_id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `core_item_tblidx` int(15) unsigned NOT NULL,
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for mail_deleted
-- ----------------------------
DROP TABLE IF EXISTS `mail_deleted`;
CREATE TABLE `mail_deleted` (
  `id` int(10) unsigned NOT NULL,
  `char_id` int(10) unsigned NOT NULL,
  `sender_type` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `mail_type` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `text_size` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `text` varchar(127) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `zenny` int(10) unsigned NOT NULL DEFAULT '0',
  `item_id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `target_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `from_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `is_accept` tinyint(1) NOT NULL DEFAULT '0',
  `is_lock` tinyint(1) NOT NULL DEFAULT '0',
  `is_read` tinyint(1) NOT NULL DEFAULT '0',
  `create_time` bigint(20) unsigned DEFAULT NULL,
  `end_time` bigint(20) unsigned DEFAULT NULL,
  `remain_day` tinyint(2) unsigned NOT NULL DEFAULT '1',
  `year` int(4) unsigned NOT NULL,
  `month` tinyint(2) unsigned NOT NULL,
  `day` tinyint(2) unsigned NOT NULL,
  `hour` tinyint(2) unsigned NOT NULL,
  `minute` tinyint(2) unsigned NOT NULL,
  `second` tinyint(2) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`,`char_id`) USING BTREE,
  UNIQUE KEY `id_2` (`id`) USING BTREE,
  KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for mute_log
-- ----------------------------
DROP TABLE IF EXISTS `mute_log`;
CREATE TABLE `mute_log` (
  `char_id` int(15) unsigned NOT NULL,
  `gm_account_id` int(15) unsigned DEFAULT NULL,
  `duration_in_minutes` int(15) unsigned DEFAULT NULL,
  `reason` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `mute_until` bigint(30) unsigned DEFAULT NULL,
  `date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`char_id`),
  UNIQUE KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for privateshoplogs
-- ----------------------------
DROP TABLE IF EXISTS `privateshoplogs`;
CREATE TABLE `privateshoplogs` (
  `id` int(15) unsigned NOT NULL AUTO_INCREMENT,
  `seller_char_id` int(15) unsigned DEFAULT NULL,
  `buyer_char_id` int(15) unsigned DEFAULT NULL,
  `zeni` int(15) unsigned DEFAULT NULL,
  `item_count` int(3) unsigned DEFAULT NULL,
  `item_id_1` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_1` int(15) unsigned DEFAULT NULL,
  `item_id_2` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_2` int(15) unsigned DEFAULT NULL,
  `item_id_3` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_3` int(15) unsigned DEFAULT NULL,
  `item_id_4` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_4` int(15) unsigned DEFAULT NULL,
  `item_id_5` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_5` int(15) unsigned DEFAULT NULL,
  `item_id_6` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_6` int(15) unsigned DEFAULT NULL,
  `item_id_7` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_7` int(15) unsigned DEFAULT NULL,
  `item_id_8` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_8` int(15) unsigned DEFAULT NULL,
  `item_id_9` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_9` int(15) unsigned DEFAULT NULL,
  `item_id_10` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_10` int(15) unsigned DEFAULT NULL,
  `item_id_11` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_11` int(15) unsigned DEFAULT NULL,
  `item_id_12` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_12` int(15) unsigned DEFAULT NULL,
  `has_issues` int(1) DEFAULT NULL,
  `issue_reason` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for slot_machine_log
-- ----------------------------
DROP TABLE IF EXISTS `slot_machine_log`;
CREATE TABLE `slot_machine_log` (
  `account_id` int(15) unsigned DEFAULT NULL,
  `char_id` int(15) unsigned DEFAULT NULL,
  `extract_count` int(3) unsigned DEFAULT NULL,
  `type` int(3) unsigned DEFAULT NULL,
  `coin` int(15) DEFAULT NULL,
  `current_points` int(15) unsigned DEFAULT NULL,
  `new_points` int(15) unsigned DEFAULT NULL,
  `product_id_1` int(15) unsigned DEFAULT NULL,
  `product_id_2` int(15) unsigned DEFAULT NULL,
  `product_id_3` int(15) unsigned DEFAULT NULL,
  `product_id_4` int(15) unsigned DEFAULT NULL,
  `product_id_5` int(15) unsigned DEFAULT NULL,
  `product_id_6` int(15) unsigned DEFAULT NULL,
  `product_id_7` int(15) unsigned DEFAULT NULL,
  `product_id_8` int(15) unsigned DEFAULT NULL,
  `product_id_9` int(15) unsigned DEFAULT NULL,
  `product_id_10` int(15) unsigned DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for tradelogs
-- ----------------------------
DROP TABLE IF EXISTS `tradelogs`;
CREATE TABLE `tradelogs` (
  `id` int(15) unsigned NOT NULL AUTO_INCREMENT,
  `char_id` int(15) unsigned DEFAULT NULL,
  `target_char_id` int(15) DEFAULT NULL,
  `zeni` int(15) unsigned DEFAULT NULL,
  `item_count` int(3) unsigned DEFAULT NULL,
  `item_id_1` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_1` int(15) unsigned DEFAULT NULL,
  `item_id_2` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_2` int(15) unsigned DEFAULT NULL,
  `item_id_3` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_3` int(15) unsigned DEFAULT NULL,
  `item_id_4` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_4` int(15) unsigned DEFAULT NULL,
  `item_id_5` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_5` int(15) unsigned DEFAULT NULL,
  `item_id_6` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_6` int(15) unsigned DEFAULT NULL,
  `item_id_7` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_7` int(15) unsigned DEFAULT NULL,
  `item_id_8` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_8` int(15) unsigned DEFAULT NULL,
  `item_id_9` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_9` int(15) unsigned DEFAULT NULL,
  `item_id_10` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_10` int(15) unsigned DEFAULT NULL,
  `item_id_11` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_11` int(15) unsigned DEFAULT NULL,
  `item_id_12` bigint(15) unsigned DEFAULT NULL,
  `item_tblidx_12` int(15) unsigned DEFAULT NULL,
  `timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
