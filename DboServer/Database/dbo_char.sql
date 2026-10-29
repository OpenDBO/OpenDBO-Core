/*
Navicat MySQL Data Transfer

Source Server         : db
Source Server Version : 80018
Source Host           : localhost:3306
Source Database       : dbo_char

Target Server Type    : MYSQL
Target Server Version : 80018
File Encoding         : 65001

Date: 2020-04-21 14:06:18
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for auctionhouse
-- ----------------------------
DROP TABLE IF EXISTS `auctionhouse`;
CREATE TABLE `auctionhouse` (
  `id` bigint(20) unsigned NOT NULL,
  `char_id` int(10) unsigned NOT NULL DEFAULT '0',
  `tab_type` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `item_name` varchar(33) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `seller_name` varchar(17) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `price` int(10) unsigned NOT NULL,
  `item_id` bigint(20) unsigned NOT NULL,
  `time_start` int(10) unsigned NOT NULL,
  `time_end` int(10) unsigned DEFAULT NULL COMMENT 'time in seconds',
  `item_level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `need_class` int(10) unsigned NOT NULL,
  `item_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`char_id`),
  UNIQUE KEY `id` (`id`) USING BTREE,
  KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Table structure for bannword
-- ----------------------------
DROP TABLE IF EXISTS `bannword`;
CREATE TABLE `bannword` (
  `id` int(5) NOT NULL AUTO_INCREMENT,
  `word` varchar(50) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for bind
-- ----------------------------
DROP TABLE IF EXISTS `bind`;
CREATE TABLE `bind` (
  `char_id` int(10) unsigned NOT NULL,
  `world_id` int(10) unsigned NOT NULL DEFAULT '1',
  `bind_object_tblidx` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `loc_x` float(11,6) NOT NULL,
  `loc_y` float(11,6) NOT NULL,
  `loc_z` float(11,6) NOT NULL,
  `dir_x` float(11,6) NOT NULL,
  `dir_y` float(11,6) NOT NULL,
  `dir_z` float(11,6) NOT NULL,
  `type` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`char_id`),
  UNIQUE KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for buffs
-- ----------------------------
DROP TABLE IF EXISTS `buffs`;
CREATE TABLE `buffs` (
  `char_id` int(10) unsigned NOT NULL,
  `source_tblidx` int(10) unsigned NOT NULL,
  `source_type` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT '0 skill and 1 item',
  `buff_index` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `buff_group` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `initial_duration` int(10) unsigned NOT NULL DEFAULT '0',
  `time_remaining` int(10) unsigned NOT NULL DEFAULT '0',
  `effect_value_1` double(10,2) DEFAULT NULL,
  `effect_value_2` double(10,2) DEFAULT NULL,
  `argument_1_0` int(10) unsigned DEFAULT NULL COMMENT 'commonConfigTblidx',
  `argument_1_1` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainTime',
  `argument_1_2` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainValue',
  `argument_2_0` int(10) unsigned DEFAULT NULL COMMENT 'commonConfigTblidx',
  `argument_2_1` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainTime',
  `argument_2_2` int(10) unsigned DEFAULT NULL COMMENT 'dwRemainValue',
  PRIMARY KEY (`char_id`,`buff_index`),
  UNIQUE KEY `char_id` (`char_id`,`buff_index`) USING BTREE,
  KEY `char_id_2` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for characters
-- ----------------------------
DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `id` int(10) unsigned NOT NULL,
  `char_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `account_id` int(10) unsigned NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `exp` int(10) unsigned NOT NULL DEFAULT '0',
  `race` tinyint(1) unsigned DEFAULT NULL,
  `class` tinyint(2) unsigned DEFAULT NULL,
  `gender` tinyint(1) unsigned DEFAULT NULL,
  `face` tinyint(2) unsigned DEFAULT NULL,
  `adult` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `hair` tinyint(2) unsigned NOT NULL,
  `hair_color` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `skin_color` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `blood` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `cur_loc_x` float(11,6) NOT NULL DEFAULT '78.900002',
  `cur_loc_y` float(11,6) NOT NULL DEFAULT '46.950001',
  `cur_loc_z` float(11,6) NOT NULL DEFAULT '168.350006',
  `cur_dir_x` float(11,6) NOT NULL DEFAULT '0.950000',
  `cur_dir_y` float(11,6) NOT NULL DEFAULT '0.000000',
  `cur_dir_z` float(11,6) NOT NULL DEFAULT '0.300000',
  `world_id` int(10) unsigned NOT NULL DEFAULT '1',
  `world_table` int(10) unsigned NOT NULL DEFAULT '1',
  `map_info_index` int(10) unsigned NOT NULL DEFAULT '0',
  `money` int(10) unsigned NOT NULL DEFAULT '0',
  `money_bank` int(10) unsigned NOT NULL DEFAULT '0',
  `tutorial_flag` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0 = start tutorial / 1 = dont start tutorial',
  `tutorial_hint` int(10) unsigned NOT NULL DEFAULT '0',
  `name_change` tinyint(1) NOT NULL DEFAULT '0',
  `reputation` int(10) unsigned NOT NULL DEFAULT '0',
  `mudosa_point` int(10) unsigned NOT NULL DEFAULT '0',
  `sp_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'skill points',
  `game_master` tinyint(1) NOT NULL DEFAULT '0',
  `guild_id` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `cur_lp` int(10) NOT NULL DEFAULT '15000',
  `cur_ep` smallint(5) unsigned NOT NULL DEFAULT '15000',
  `cur_rp` smallint(5) unsigned NOT NULL DEFAULT '0',
  `cur_ap` int(10) NOT NULL DEFAULT '450000',
  `mail_is_away` tinyint(1) NOT NULL DEFAULT '0',
  `srv_farm_id` int(3) unsigned NOT NULL DEFAULT '0',
  `del_char_time` bigint(20) unsigned DEFAULT NULL COMMENT 'time(0) + 43200 = 12 hours',
  `hoipoi_normal_start` tinyint(1) NOT NULL DEFAULT '0',
  `hoipoi_special_start` tinyint(1) NOT NULL DEFAULT '0',
  `hoipoi_type` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `hoipoi_mix_level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `hoipoi_mix_exp` int(10) unsigned NOT NULL DEFAULT '0',
  `title` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `mascot` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `rp_ball` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `netpy` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Netpy are points the user receives while staying online',
  `wagu_point` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '',
  `ip` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '0.0.0.0' COMMENT 'the last IP in the char',
  `air_state` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '0 = off and 1 = on',
  `invisible_costume` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0 = false 1 = true',
  `play_time` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'play time in seconds',
  `superior_effect_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `create_time` bigint(15) unsigned NOT NULL COMMENT 'time(0)',
  `is_online` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`char_name`,`srv_farm_id`),
  UNIQUE KEY `id` (`id`) USING BTREE,
  UNIQUE KEY `char_name` (`char_name`) USING BTREE,
  UNIQUE KEY `id_2` (`id`,`srv_farm_id`) USING BTREE,
  UNIQUE KEY `id_3` (`id`,`account_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for dojos
-- ----------------------------
DROP TABLE IF EXISTS `dojos`;
CREATE TABLE `dojos` (
  `guild_id` int(10) unsigned NOT NULL,
  `dojo_tblidx` int(10) unsigned NOT NULL,
  `level` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `peace_status` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `peace_points` int(15) unsigned NOT NULL DEFAULT '0',
  `guild_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `leader_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `notice` varchar(64) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `challenge_guild_id` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `seed_char_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  PRIMARY KEY (`dojo_tblidx`),
  UNIQUE KEY `dojo_tblidx` (`dojo_tblidx`) USING BTREE,
  UNIQUE KEY `guild_id` (`guild_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for dwc_limit
-- ----------------------------
DROP TABLE IF EXISTS `dwc_limit`;
CREATE TABLE `dwc_limit` (
  `char_id` int(10) unsigned NOT NULL,
  `enter_count` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `enter_day` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'YYYYMMDD of the daily period (resets at 04:00) enter_count belongs to',
  PRIMARY KEY (`char_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for dwc_scenario
-- ----------------------------
DROP TABLE IF EXISTS `dwc_scenario`;
CREATE TABLE `dwc_scenario` (
  `char_id` int(10) unsigned NOT NULL,
  `dwc_tblidx` int(10) unsigned NOT NULL,
  `mission_index` tinyint(3) unsigned NOT NULL COMMENT 'index into sDWC_TBLDAT::aMissionTblidx',
  `reward_grade` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT 'highest card grade obtained, 255 = none',
  `clear_count` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`,`dwc_tblidx`,`mission_index`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for friendlist
-- ----------------------------
DROP TABLE IF EXISTS `friendlist`;
CREATE TABLE `friendlist` (
  `char_id` int(10) unsigned NOT NULL,
  `friend_char_id` int(10) unsigned NOT NULL,
  `friend_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `blacklist` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`,`friend_char_id`),
  UNIQUE KEY `char_id` (`char_id`,`friend_char_id`) USING BTREE,
  KEY `char_id_2` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for guilds
-- ----------------------------
DROP TABLE IF EXISTS `guilds`;
CREATE TABLE `guilds` (
  `id` int(10) unsigned NOT NULL,
  `guild_name` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL,
  `master_char_id` int(10) unsigned NOT NULL,
  `second_master_char_id` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `second_master_2_char_id` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `second_master_3_char_id` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `second_master_4_char_id` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `guild_reputation` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_point_ever` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'max guild points ever received',
  `function_flag` bigint(15) unsigned NOT NULL DEFAULT '7',
  `guild_disband_time` int(15) unsigned DEFAULT NULL,
  `mark_in_color` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `mark_in_line` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `mark_main` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `mark_main_color` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `mark_out_color` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `mark_out_line` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `notice_by` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `guild_notice` varchar(257) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `dojo_color` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `guild_color` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `dogi_type` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `zeni` int(10) unsigned DEFAULT '0' COMMENT 'Zeni inside guild bank',
  PRIMARY KEY (`id`,`guild_name`),
  UNIQUE KEY `id` (`id`) USING BTREE,
  UNIQUE KEY `guild_name` (`guild_name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for guild_members
-- ----------------------------
DROP TABLE IF EXISTS `guild_members`;
CREATE TABLE `guild_members` (
  `guild_id` int(14) unsigned NOT NULL,
  `char_id` int(14) unsigned NOT NULL,
  PRIMARY KEY (`guild_id`,`char_id`),
  UNIQUE KEY `guild_id` (`guild_id`,`char_id`) USING BTREE,
  UNIQUE KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for hoipoi_recipe
-- ----------------------------
DROP TABLE IF EXISTS `hoipoi_recipe`;
CREATE TABLE `hoipoi_recipe` (
  `char_id` int(10) unsigned NOT NULL,
  `recipe_tblidx` int(10) unsigned NOT NULL,
  `recipe_type` tinyint(3) unsigned NOT NULL DEFAULT '255',
  PRIMARY KEY (`char_id`,`recipe_tblidx`),
  UNIQUE KEY `char_id_2` (`char_id`,`recipe_tblidx`) USING BTREE,
  KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for htb_skills
-- ----------------------------
DROP TABLE IF EXISTS `htb_skills`;
CREATE TABLE `htb_skills` (
  `skill_id` int(10) unsigned NOT NULL DEFAULT '0',
  `char_id` int(10) unsigned NOT NULL,
  `slot_id` smallint(3) unsigned NOT NULL DEFAULT '0',
  `time_remaining` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`skill_id`,`char_id`),
  UNIQUE KEY `skill_id` (`skill_id`,`char_id`) USING BTREE,
  UNIQUE KEY `char_id` (`char_id`,`slot_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for items
-- ----------------------------
DROP TABLE IF EXISTS `items`;
CREATE TABLE `items` (
  `id` bigint(20) unsigned NOT NULL,
  `tblidx` int(10) unsigned NOT NULL DEFAULT '0',
  `char_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'character id',
  `place` tinyint(1) unsigned NOT NULL DEFAULT '1',
  `pos` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `count` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `rank` tinyint(2) NOT NULL DEFAULT '1',
  `durability` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `grade` tinyint(2) NOT NULL DEFAULT '0',
  `need_to_identify` tinyint(1) NOT NULL DEFAULT '0',
  `battle_attribute` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `maker` varchar(16) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '',
  `option_tblidx` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT 'item_option_data tblidx',
  `option_tblidx_2` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT 'item_option_data tblidx',
  `option_random_id` smallint(5) unsigned NOT NULL DEFAULT '65535' COMMENT 'item_enchant tblidx',
  `option_random_val` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `option_random_id_2` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `option_random_val_2` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `option_random_id_3` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `option_random_val_3` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `option_random_id_4` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `option_random_val_4` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `option_random_id_5` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `option_random_val_5` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `option_random_id_6` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `option_random_val_6` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `option_random_id_7` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `option_random_val_7` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `option_random_id_8` smallint(5) unsigned NOT NULL DEFAULT '65535',
  `option_random_val_8` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `use_start_time` bigint(15) unsigned NOT NULL DEFAULT '0' COMMENT 'max duration time (3600 = 60 minutes)',
  `use_end_time` bigint(15) unsigned NOT NULL DEFAULT '0' COMMENT 'current duration time (3600 = 60 minutes)',
  `restrict_state` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `duration_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `account_id` int(10) unsigned DEFAULT '0',
  `guild_id` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`id`,`char_id`,`place`,`pos`),
  UNIQUE KEY `id` (`id`) USING BTREE,
  KEY `char_id_idx` (`char_id`) USING BTREE,
  KEY `item_vnum_index` (`tblidx`) USING BTREE,
  KEY `char_id` (`char_id`,`place`,`pos`) USING BTREE,
  KEY `char_id_2` (`char_id`,`place`) USING BTREE,
  KEY `account_id` (`account_id`) USING BTREE,
  KEY `guild_id` (`guild_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for items_cd
-- ----------------------------
DROP TABLE IF EXISTS `items_cd`;
CREATE TABLE `items_cd` (
  `char_id` int(10) unsigned NOT NULL,
  `group_index` tinyint(2) unsigned NOT NULL DEFAULT '0',
  `cool_time` int(10) unsigned NOT NULL DEFAULT '0',
  `time_remaining` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`group_index`,`char_id`),
  UNIQUE KEY `char_id` (`char_id`,`group_index`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for mail
-- ----------------------------
DROP TABLE IF EXISTS `mail`;
CREATE TABLE `mail` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
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
-- Table structure for mascots
-- ----------------------------
DROP TABLE IF EXISTS `mascots`;
CREATE TABLE `mascots` (
  `char_id` int(15) unsigned NOT NULL,
  `slot_id` tinyint(3) unsigned NOT NULL DEFAULT '255' COMMENT 'index',
  `mascot_tblidx` int(10) unsigned NOT NULL,
  `cur_vp` int(10) unsigned NOT NULL DEFAULT '100',
  `max_vp` int(10) unsigned NOT NULL DEFAULT '100',
  `cur_exp` int(10) unsigned NOT NULL DEFAULT '0',
  `skill_tblidx_0` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `skill_tblidx_1` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `skill_tblidx_2` int(10) unsigned NOT NULL DEFAULT '4294967295',
  `skill_tblidx_3` int(10) unsigned NOT NULL DEFAULT '4294967295',
  PRIMARY KEY (`char_id`,`slot_id`),
  UNIQUE KEY `char_id` (`char_id`,`slot_id`) USING BTREE,
  KEY `char_id_2` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for portals
-- ----------------------------
DROP TABLE IF EXISTS `portals`;
CREATE TABLE `portals` (
  `char_id` int(10) unsigned NOT NULL,
  `point` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`char_id`,`point`),
  UNIQUE KEY `char_id_2` (`char_id`,`point`) USING BTREE,
  KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for questitems
-- ----------------------------
DROP TABLE IF EXISTS `questitems`;
CREATE TABLE `questitems` (
  `char_id` int(10) unsigned NOT NULL,
  `tblidx` int(10) unsigned NOT NULL,
  `amount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `pos` tinyint(3) unsigned NOT NULL DEFAULT '255',
  KEY `char_id_2` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for quests
-- ----------------------------
DROP TABLE IF EXISTS `quests`;
CREATE TABLE `quests` (
  `char_id` int(10) unsigned NOT NULL,
  `quest_id` int(10) unsigned NOT NULL,
  `tc_quest_info` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `ta_quest_info` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tg_exc_c_group` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tc_pre_id` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tc_cur_id` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `tc_id` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `ta_id` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `evt_user_data` int(10) unsigned NOT NULL DEFAULT '0',
  `evt_user_data_2` int(10) unsigned NOT NULL DEFAULT '0',
  `evt_user_data_3` int(10) unsigned NOT NULL DEFAULT '0',
  `evt_user_data_4` int(10) unsigned NOT NULL DEFAULT '0',
  `tc_time_info` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `ta_time_info` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `time_left` int(10) unsigned NOT NULL DEFAULT '0',
  `q_state` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`,`quest_id`),
  UNIQUE KEY `char_id_2` (`char_id`,`quest_id`) USING BTREE,
  KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for quickslot
-- ----------------------------
DROP TABLE IF EXISTS `quickslot`;
CREATE TABLE `quickslot` (
  `char_id` int(10) unsigned NOT NULL DEFAULT '0',
  `tblidx` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'item unique id',
  PRIMARY KEY (`char_id`,`slot`),
  UNIQUE KEY `char_id` (`char_id`,`slot`) USING BTREE,
  KEY `char_id_2` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for quick_teleport
-- ----------------------------
DROP TABLE IF EXISTS `quick_teleport`;
CREATE TABLE `quick_teleport` (
  `char_id` int(10) unsigned NOT NULL,
  `slot_num` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `world_tblidx` int(10) unsigned NOT NULL,
  `loc_x` float(11,6) NOT NULL,
  `loc_y` float(11,6) NOT NULL,
  `loc_z` float(11,6) NOT NULL,
  `map_name_tblidx` int(10) unsigned NOT NULL,
  `day` tinyint(2) unsigned NOT NULL,
  `hour` tinyint(2) unsigned NOT NULL,
  `minute` tinyint(2) unsigned NOT NULL,
  `month` tinyint(2) unsigned NOT NULL,
  `second` tinyint(2) unsigned NOT NULL,
  `year` int(4) unsigned NOT NULL,
  PRIMARY KEY (`char_id`,`slot_num`),
  UNIQUE KEY `char_id` (`char_id`,`slot_num`) USING BTREE,
  KEY `char_id_2` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for rank_battle
-- ----------------------------
DROP TABLE IF EXISTS `rank_battle`;
CREATE TABLE `rank_battle` (
  `char_id` int(10) unsigned NOT NULL,
  `win` int(10) unsigned NOT NULL DEFAULT '0',
  `draw` int(10) unsigned NOT NULL DEFAULT '0',
  `lose` int(10) unsigned NOT NULL DEFAULT '0',
  `straight_ko_win` int(10) unsigned NOT NULL DEFAULT '0',
  `max_straight_ko_win` int(10) unsigned NOT NULL DEFAULT '0',
  `max_straight_win` int(10) unsigned NOT NULL DEFAULT '0',
  `straight_win` int(10) unsigned NOT NULL DEFAULT '0',
  `points` float(10,0) NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`),
  UNIQUE KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for skills
-- ----------------------------
DROP TABLE IF EXISTS `skills`;
CREATE TABLE `skills` (
  `skill_id` int(10) unsigned NOT NULL DEFAULT '0',
  `char_id` int(10) unsigned NOT NULL,
  `rp_bonus_auto` tinyint(1) NOT NULL DEFAULT '0',
  `rp_bonus_type` tinyint(3) unsigned NOT NULL DEFAULT '255',
  `slot_id` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'skillIndex',
  `time_remaining` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Skill CD Time',
  `exp` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`char_id`,`slot_id`),
  UNIQUE KEY `char_id` (`char_id`,`slot_id`) USING BTREE,
  KEY `char_id_2` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for sky_dungeon_limit
-- ----------------------------
DROP TABLE IF EXISTS `sky_dungeon_limit`;
CREATE TABLE `sky_dungeon_limit` (
  `char_id` int(10) unsigned NOT NULL,
  `enter_count` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `enter_day` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'YYYYMMDD of the daily period (resets at 04:00) enter_count belongs to',
  PRIMARY KEY (`char_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for titles
-- ----------------------------
DROP TABLE IF EXISTS `titles`;
CREATE TABLE `titles` (
  `char_id` int(10) unsigned NOT NULL,
  `title_tblidx` int(10) unsigned NOT NULL,
  PRIMARY KEY (`char_id`,`title_tblidx`),
  UNIQUE KEY `char_id_2` (`char_id`,`title_tblidx`) USING BTREE,
  KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for warfog
-- ----------------------------
DROP TABLE IF EXISTS `warfog`;
CREATE TABLE `warfog` (
  `char_id` int(10) unsigned NOT NULL,
  `war_fog` int(10) unsigned NOT NULL,
  PRIMARY KEY (`char_id`,`war_fog`),
  UNIQUE KEY `char_id_2` (`char_id`,`war_fog`) USING BTREE,
  KEY `char_id` (`char_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
