-- =====================================================================
-- seeds.sql
--
-- PURPOSE:
--   Seeds 12 local test accounts (admin1 .. admin12), each with full
--   GM access and a single level 70 character of a different master
--   class:
--
--     admin1  Fighter            admin7  Dende Priest
--     admin2  Swordsman          admin8  Poko Priest
--     admin3  Crane Hermit       admin9  Ultimate Majin
--     admin4  Turtle Hermit      admin10 Grand Chef Majin
--     admin5  Dark Warrior       admin11 Plasma Majin
--     admin6  Shadow Knight      admin12 Karma Majin
--
--   Every character gets:
--     - Dark Soul armor set +15 (physical variant for fighter classes,
--       energy variant for mystic classes)
--     - Flame weapon + sub weapon +15 (CC150)
--     - Corona necklace + 2 Corona earrings (Scramble)
--     - Octopus Alpha Defense + Alpha Attack rings (Kraken CON combo)
--     - Perfect Scouter
--     - Basic Capsule Kit + HL Capsule Box Z32 in the other 4 bag slots
--     - TLQ passives (Power Up, Guard, Dash, Counter Attack), Flight,
--       the master class mastery passive and the newbie class skills
--     - race transformation and all 6 dragon buffs (Shenron skill wishes)
--     - 1,000,000,000 zeni, 10,000 tokens and 10,000 WP
--     - every teleport point, the whole warfog and every title unlocked
--     - spawn and bind point at Kokkara Village
--
--   Two guilds with every guild/dojo function unlocked, max points and
--   1,000,000,000 zeni in the guild bank:
--     OpenDBO   admin1 .. admin6  (master admin1)
--     OpenDBO2  admin7 .. admin12 (master admin7)
--
--   Login: admin1 .. admin12 / password "password".
--
-- NOTES:
--   - Re-runnable: the accounts are upserted and every seeded character
--     (ids 900001 .. 900012) is wiped and recreated.
--   - Characters and guilds use fixed ids 900001 .. 900012 / 900001 ..
--     900002 and items use ids from 900000000 up. QueryServer picks
--     MAX(id) + 1 for new rows on start, so restart it after running
--     this script.
--   - Local/dev databases only.
-- =====================================================================

-- SHA3-256 hash of "password"
SET @password_hash = 'c0067d4af4e87f00dbac63b6156828237059172d1bbeac67427345d6a9fda484';
SET @char_id_base = 900000;
SET @item_id_base = 900000000;

SET @zeni = 1000000000;
SET @tokens = 10000;		-- netpy
SET @wagu_points = 10000;	-- WP

-- Kokkara Village
SET @spawn_world = 1;
SET @spawn_map_info_index = 200101100;
SET @spawn_x = 4502.909668;
SET @spawn_y = -44.829998;
SET @spawn_z = 4018.359863;
SET @spawn_dir_x = -0.993200;
SET @spawn_dir_y = 0.0;
SET @spawn_dir_z = 0.116900;

-- guilds: OpenDBO (admin1 .. admin6) and OpenDBO2 (admin7 .. admin12)
SET @guild_id_base = 900000;
SET @guild_function_flag = 17179869183;	-- every eDBO_GUILD_FUNCTION bit (0 .. 33)
SET @guild_points = 1000000;			-- reputation and max points ever
SET @guild_zeni = 1000000000;

SET @portal_count = 61;		-- Table_Portal_Data ids 1 .. 61
SET @warfog_count = 80;		-- NTL_MAX_COUNT_WAR_FOG, content ids 0 .. 79


USE dbo_char;

-- ---------------------------------------------------------------------
-- Seed data
-- ---------------------------------------------------------------------
DROP TEMPORARY TABLE IF EXISTS seed_chars;
CREATE TEMPORARY TABLE seed_chars (
	n				TINYINT UNSIGNED PRIMARY KEY,	-- account number (admin<n>) and char id offset
	char_name		VARCHAR(16) NOT NULL,
	race			TINYINT UNSIGNED NOT NULL,		-- eRACE
	class			TINYINT UNSIGNED NOT NULL,		-- ePC_CLASS
	gender			TINYINT UNSIGNED NOT NULL,		-- eGENDER
	gloves			INT UNSIGNED NOT NULL,
	gloves_option	INT UNSIGNED NOT NULL,
	sub_weapon		INT UNSIGNED NOT NULL,
	armor_set		INT UNSIGNED NOT NULL			-- Dark Soul jacket tblidx, pants = +1, shoes = +2
);

INSERT INTO seed_chars VALUES
--  n   name               race class gender gloves   option   sub      armor
	(1,  'Fighter',         0,   7,    0,     7001468, 7001468, 7001480, 7001426),
	(2,  'Swordsman',       0,   8,    0,     7001469, 7001468, 7001481, 7001426),
	(3,  'CraneHermit',     0,   9,    0,     7001470, 7001470, 7001482, 7001429),
	(4,  'TurtleHermit',    0,   10,   0,     7001471, 7001470, 7001483, 7001429),
	(5,  'DarkWarrior',     1,   13,   2,     7001472, 7001472, 7001484, 7001432),
	(6,  'ShadowKnight',    1,   14,   2,     7001473, 7001472, 7001485, 7001432),
	(7,  'DendePriest',     1,   15,   2,     7001475, 7001474, 7001487, 7001435),
	(8,  'PokoPriest',      1,   16,   2,     7001474, 7001474, 7001486, 7001435),
	(9,  'UltimateMajin',   2,   17,   0,     7001477, 7001476, 7001489, 7001441),
	(10, 'GrandChefMajin',  2,   18,   0,     7001476, 7001476, 7001488, 7001441),
	(11, 'PlasmaMajin',     2,   19,   0,     7001478, 7001478, 7001490, 7001438),
	(12, 'KarmaMajin',      2,   20,   0,     7001479, 7001478, 7001491, 7001438);

-- skills per master class, slot_id is the position in the character skill list
DROP TEMPORARY TABLE IF EXISTS seed_skills;
CREATE TEMPORARY TABLE seed_skills (
	class		TINYINT UNSIGNED NOT NULL,
	slot_id		TINYINT UNSIGNED NOT NULL,
	skill_id	INT UNSIGNED NOT NULL
);

-- slot 0-2: newbie skills | 3: Power Up | 4: Guard | 5: Dash | 6: Counter Attack | 7: Flight | 8: master class mastery
INSERT INTO seed_skills VALUES
	(7,  0, 10311),  (7,  1, 20091),  (7,  2, 20111),  (7,  3, 20221),  (7,  4, 20201),  (7,  5, 20211),  (7,  6, 10021),  (7,  7, 20911),  (7,  8, 729991),
	(8,  0, 10311),  (8,  1, 20091),  (8,  2, 20111),  (8,  3, 20221),  (8,  4, 20201),  (8,  5, 20211),  (8,  6, 10021),  (8,  7, 20911),  (8,  8, 829991),
	(9,  0, 110111), (9,  1, 120131), (9,  2, 110011), (9,  3, 120501), (9,  4, 120301), (9,  5, 120311), (9,  6, 110051), (9,  7, 120911), (9,  8, 929991),
	(10, 0, 110111), (10, 1, 120131), (10, 2, 110011), (10, 3, 120501), (10, 4, 120301), (10, 5, 120311), (10, 6, 110051), (10, 7, 120911), (10, 8, 1029991),
	(13, 0, 310111), (13, 1, 320101), (13, 2, 320111), (13, 3, 320321), (13, 4, 320201), (13, 5, 320311), (13, 6, 310021), (13, 7, 320811), (13, 8, 1329991),
	(14, 0, 310111), (14, 1, 320101), (14, 2, 320111), (14, 3, 320321), (14, 4, 320201), (14, 5, 320311), (14, 6, 310021), (14, 7, 320811), (14, 8, 1429991),
	(15, 0, 410111), (15, 1, 420141), (15, 2, 420111), (15, 3, 420421), (15, 4, 420301), (15, 5, 420411), (15, 6, 410011), (15, 7, 421111), (15, 8, 1529991),
	(16, 0, 410111), (16, 1, 420141), (16, 2, 420111), (16, 3, 420421), (16, 4, 420301), (16, 5, 420411), (16, 6, 410011), (16, 7, 421111), (16, 8, 1629991),
	(17, 0, 510111), (17, 1, 521171), (17, 2, 510121), (17, 3, 520161), (17, 4, 520131), (17, 5, 520321), (17, 6, 510031), (17, 7, 520241), (17, 8, 1729991),
	(18, 0, 510111), (18, 1, 521171), (18, 2, 510121), (18, 3, 520161), (18, 4, 520131), (18, 5, 520321), (18, 6, 510031), (18, 7, 520241), (18, 8, 1829991),
	(19, 0, 610111), (19, 1, 620101), (19, 2, 620111), (19, 3, 620161), (19, 4, 620131), (19, 5, 620151), (19, 6, 610021), (19, 7, 620231), (19, 8, 1929991),
	(20, 0, 610111), (20, 1, 620101), (20, 2, 620111), (20, 3, 620161), (20, 4, 620131), (20, 5, 620151), (20, 6, 610021), (20, 7, 620231), (20, 8, 2029991);

-- slot 9: race transformation (Super Saiyan / Great Namek / Pure Majin)
-- slot 10-15: dragon buffs (Dragon King's Focus, Energy, Constitution, Strength, Soul, Dexterity)
INSERT INTO seed_skills VALUES
	(7,  9, 21011),  (7,  10, 99100), (7,  11, 99110), (7,  12, 99120), (7,  13, 99130), (7,  14, 99140), (7,  15, 99150),
	(8,  9, 21011),  (8,  10, 99100), (8,  11, 99110), (8,  12, 99120), (8,  13, 99130), (8,  14, 99140), (8,  15, 99150),
	(9,  9, 121111), (9,  10, 99101), (9,  11, 99111), (9,  12, 99121), (9,  13, 99131), (9,  14, 99141), (9,  15, 99151),
	(10, 9, 121111), (10, 10, 99101), (10, 11, 99111), (10, 12, 99121), (10, 13, 99131), (10, 14, 99141), (10, 15, 99151),
	(13, 9, 320911), (13, 10, 99103), (13, 11, 99113), (13, 12, 99123), (13, 13, 99133), (13, 14, 99143), (13, 15, 99153),
	(14, 9, 320911), (14, 10, 99103), (14, 11, 99113), (14, 12, 99123), (14, 13, 99133), (14, 14, 99143), (14, 15, 99153),
	(15, 9, 421211), (15, 10, 99104), (15, 11, 99114), (15, 12, 99124), (15, 13, 99134), (15, 14, 99144), (15, 15, 99154),
	(16, 9, 421211), (16, 10, 99104), (16, 11, 99114), (16, 12, 99124), (16, 13, 99134), (16, 14, 99144), (16, 15, 99154),
	(17, 9, 520251), (17, 10, 99105), (17, 11, 99115), (17, 12, 99125), (17, 13, 99135), (17, 14, 99145), (17, 15, 99155),
	(18, 9, 520251), (18, 10, 99105), (18, 11, 99115), (18, 12, 99125), (18, 13, 99135), (18, 14, 99145), (18, 15, 99155),
	(19, 9, 620241), (19, 10, 99106), (19, 11, 99116), (19, 12, 99126), (19, 13, 99136), (19, 14, 99146), (19, 15, 99156),
	(20, 9, 620241), (20, 10, 99106), (20, 11, 99116), (20, 12, 99126), (20, 13, 99136), (20, 14, 99146), (20, 15, 99156);

-- items shared by every character (class specific gear is added from seed_chars)
-- place: 0 = bag slot, 6 = equip, 7 = bank slot | restrict_state 2 = character bound
DROP TEMPORARY TABLE IF EXISTS seed_items;
CREATE TEMPORARY TABLE seed_items (
	place			TINYINT UNSIGNED NOT NULL,
	pos				TINYINT UNSIGNED NOT NULL,
	tblidx			INT UNSIGNED NOT NULL,
	item_rank		TINYINT UNSIGNED NOT NULL,
	durability		TINYINT UNSIGNED NOT NULL,
	option_tblidx	INT UNSIGNED NOT NULL,
	restrict_state	TINYINT UNSIGNED NOT NULL
);

INSERT INTO seed_items VALUES
	-- Basic Capsule Kit in the default bag slot
	(0, 0,  19901,    1, 255, 4294967295, 2),
	-- HL Capsule Box Z32 [Permanent] in the other bag slots
	(0, 1,  403043,   1, 255, 4294967295, 2),
	(0, 2,  403043,   1, 255, 4294967295, 2),
	(0, 3,  403043,   1, 255, 4294967295, 2),
	(0, 4,  403043,   1, 255, 4294967295, 2),
	-- Basic Warehouse
	(7, 0,  19991,    1, 255, 4294967295, 0),
	-- Perfect Scouter
	(6, 5,  60000053, 3, 200, 60000053,   2),
	-- Corona Necklace + Corona Earrings (Scramble)
	(6, 7,  889004,   5, 255, 889004,     2),
	(6, 8,  891013,   5, 255, 891013,     2),
	(6, 9,  891013,   5, 255, 891013,     2),
	-- Octopus Alpha Defense Ring + Octopus Alpha Attack Ring (Kraken CON combo)
	(6, 10, 31000009, 5, 255, 31000009,   2),
	(6, 11, 31000010, 5, 255, 31000010,   2);

-- 0 .. 79, used for portal ids and warfog content ids
DROP TEMPORARY TABLE IF EXISTS seed_numbers;
CREATE TEMPORARY TABLE seed_numbers (
	num			TINYINT UNSIGNED PRIMARY KEY
);

INSERT INTO seed_numbers VALUES
	(0),  (1),  (2),  (3),  (4),  (5),  (6),  (7),  (8),  (9),
	(10), (11), (12), (13), (14), (15), (16), (17), (18), (19),
	(20), (21), (22), (23), (24), (25), (26), (27), (28), (29),
	(30), (31), (32), (33), (34), (35), (36), (37), (38), (39),
	(40), (41), (42), (43), (44), (45), (46), (47), (48), (49),
	(50), (51), (52), (53), (54), (55), (56), (57), (58), (59),
	(60), (61), (62), (63), (64), (65), (66), (67), (68), (69),
	(70), (71), (72), (73), (74), (75), (76), (77), (78), (79);

-- every title in Table_CharTitle_Data
DROP TEMPORARY TABLE IF EXISTS seed_titles;
CREATE TEMPORARY TABLE seed_titles (
	title_tblidx	INT UNSIGNED PRIMARY KEY
);

INSERT INTO seed_titles VALUES
	(1),   (2),   (3),   (4),   (5),   (6),   (7),   (8),   (9),   (10),
	(201), (202), (203), (204), (205), (206), (207), (208), (209), (210),
	(211), (212), (213), (214), (215), (216),
	(300), (301), (302), (303),
	(477), (478), (479), (480), (481), (482), (483), (484), (485), (486),
	(487), (488), (489), (490), (491), (492), (493), (494), (495), (496),
	(497), (498), (499), (500), (501), (502), (503), (504), (505), (506),
	(507), (508), (509), (510), (511), (512);


-- ---------------------------------------------------------------------
-- Accounts: upsert admin1 .. admin12 with full GM access
-- ---------------------------------------------------------------------
INSERT INTO dbo_acc.accounts (username, password_hash, acc_status, email, admin, is_gm, del_char_pw, last_ip)
SELECT CONCAT('admin', n), @password_hash, 'active', 'test@mail.com', 10, 10, @password_hash, '127.0.0.1'
FROM seed_chars
ON DUPLICATE KEY UPDATE
	password_hash = VALUES(password_hash),
	acc_status = 'active',
	admin = 10,
	is_gm = 10,
	del_char_pw = VALUES(del_char_pw);


-- ---------------------------------------------------------------------
-- Wipe previously seeded characters (and anything they picked up)
-- ---------------------------------------------------------------------
SET @first_char_id = @char_id_base + 1;
SET @last_char_id = @char_id_base + 12;

DELETE FROM bind WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM buffs WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM friendlist WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM hoipoi_recipe WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM htb_skills WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM items WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM items_cd WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM mascots WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM portals WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM questitems WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM quests WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM quick_teleport WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM quickslot WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM skills WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM titles WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM warfog WHERE char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM items WHERE id >= @item_id_base AND id < @item_id_base + 100000;
DELETE FROM characters WHERE id BETWEEN @first_char_id AND @last_char_id OR char_name IN (SELECT char_name FROM seed_chars);


-- ---------------------------------------------------------------------
-- Characters: level 70 master class at Kokkara, GM flag, 69 SP (level 70 total)
-- ---------------------------------------------------------------------
INSERT INTO characters (id, char_name, account_id, level, race, class, gender, face, adult, hair, hair_color, skin_color,
	cur_loc_x, cur_loc_y, cur_loc_z, cur_dir_x, cur_dir_y, cur_dir_z, world_id, world_table, map_info_index,
	money, netpy, wagu_point, tutorial_flag, sp_point, game_master, srv_farm_id, create_time)
SELECT @char_id_base + s.n, s.char_name, a.id, 70, s.race, s.class, s.gender, 1, 1, 1, 1, 1,
	@spawn_x, @spawn_y, @spawn_z, @spawn_dir_x, @spawn_dir_y, @spawn_dir_z, @spawn_world, @spawn_world, @spawn_map_info_index,
	@zeni, @tokens, @wagu_points, 1, 69, 1, 0, UNIX_TIMESTAMP()
FROM seed_chars s
JOIN dbo_acc.accounts a ON a.username = CONCAT('admin', s.n);

INSERT INTO bind (char_id, world_id, loc_x, loc_y, loc_z, dir_x, dir_y, dir_z)
SELECT @char_id_base + n, @spawn_world, @spawn_x, @spawn_y, @spawn_z, @spawn_dir_x, @spawn_dir_y, @spawn_dir_z
FROM seed_chars;

-- ---------------------------------------------------------------------
-- Guilds: every function unlocked (190 members, 4 second masters,
-- warehouse, emblem, dogi, dojo + dojo upgrades) and max points
-- guild 1 = OpenDBO (admin1 .. admin6, master admin1)
-- guild 2 = OpenDBO2 (admin7 .. admin12, master admin7)
-- ---------------------------------------------------------------------
DELETE FROM guild_members WHERE guild_id IN (@guild_id_base + 1, @guild_id_base + 2) OR char_id BETWEEN @first_char_id AND @last_char_id;
DELETE FROM dojos WHERE guild_id IN (@guild_id_base + 1, @guild_id_base + 2);
DELETE FROM guilds WHERE id IN (@guild_id_base + 1, @guild_id_base + 2) OR guild_name IN ('OpenDBO', 'OpenDBO2');

INSERT INTO guilds (id, guild_name, master_char_id, guild_reputation, guild_point_ever, function_flag, zeni)
VALUES
	(@guild_id_base + 1, 'OpenDBO',  @char_id_base + 1, @guild_points, @guild_points, @guild_function_flag, @guild_zeni),
	(@guild_id_base + 2, 'OpenDBO2', @char_id_base + 7, @guild_points, @guild_points, @guild_function_flag, @guild_zeni);

INSERT INTO guild_members (guild_id, char_id)
SELECT @guild_id_base + IF(n <= 6, 1, 2), @char_id_base + n
FROM seed_chars;

UPDATE characters
SET guild_id = @guild_id_base + IF(id - @char_id_base <= 6, 1, 2),
	guild_name = IF(id - @char_id_base <= 6, 'OpenDBO', 'OpenDBO2')
WHERE id BETWEEN @first_char_id AND @last_char_id;

-- every teleport point
INSERT INTO portals (char_id, point)
SELECT @char_id_base + c.n, p.num
FROM seed_chars c
CROSS JOIN seed_numbers p
WHERE p.num BETWEEN 1 AND @portal_count;

-- whole warfog
INSERT INTO warfog (char_id, war_fog)
SELECT @char_id_base + c.n, w.num
FROM seed_chars c
CROSS JOIN seed_numbers w
WHERE w.num < @warfog_count;

-- every title
INSERT INTO titles (char_id, title_tblidx)
SELECT @char_id_base + c.n, t.title_tblidx
FROM seed_chars c
CROSS JOIN seed_titles t;


-- ---------------------------------------------------------------------
-- Skills
-- ---------------------------------------------------------------------
INSERT INTO skills (skill_id, char_id, slot_id)
SELECT k.skill_id, @char_id_base + c.n, k.slot_id
FROM seed_chars c
JOIN seed_skills k ON k.class = c.class;


-- ---------------------------------------------------------------------
-- Items
-- item id = @item_id_base + n * 1000 + place * 20 + pos
-- ---------------------------------------------------------------------
INSERT INTO items (id, tblidx, char_id, place, pos, count, `rank`, durability, grade, option_tblidx, restrict_state)
SELECT @item_id_base + c.n * 1000 + i.place * 20 + i.pos, i.tblidx, @char_id_base + c.n, i.place, i.pos, 1,
	i.item_rank, i.durability, 0, i.option_tblidx, i.restrict_state
FROM seed_chars c
CROSS JOIN seed_items i;

-- Flame weapon (hand) + sub weapon (CC150) +15
INSERT INTO items (id, tblidx, char_id, place, pos, count, `rank`, durability, grade, option_tblidx, restrict_state)
SELECT @item_id_base + n * 1000 + 6 * 20 + 0, gloves, @char_id_base + n, 6, 0, 1, 5, 110, 15, gloves_option, 2
FROM seed_chars;

INSERT INTO items (id, tblidx, char_id, place, pos, count, `rank`, durability, grade, option_tblidx, restrict_state)
SELECT @item_id_base + n * 1000 + 6 * 20 + 1, sub_weapon, @char_id_base + n, 6, 1, 1, 5, 110, 15, sub_weapon, 2
FROM seed_chars;

-- Dark Soul jacket, pants, shoes +15
INSERT INTO items (id, tblidx, char_id, place, pos, count, `rank`, durability, grade, option_tblidx, restrict_state)
SELECT @item_id_base + n * 1000 + 6 * 20 + 2, armor_set, @char_id_base + n, 6, 2, 1, 5, 110, 15, armor_set, 2
FROM seed_chars;

INSERT INTO items (id, tblidx, char_id, place, pos, count, `rank`, durability, grade, option_tblidx, restrict_state)
SELECT @item_id_base + n * 1000 + 6 * 20 + 3, armor_set + 1, @char_id_base + n, 6, 3, 1, 5, 110, 15, armor_set + 1, 2
FROM seed_chars;

INSERT INTO items (id, tblidx, char_id, place, pos, count, `rank`, durability, grade, option_tblidx, restrict_state)
SELECT @item_id_base + n * 1000 + 6 * 20 + 4, armor_set + 2, @char_id_base + n, 6, 4, 1, 5, 110, 15, armor_set + 2, 2
FROM seed_chars;


DROP TEMPORARY TABLE IF EXISTS seed_chars;
DROP TEMPORARY TABLE IF EXISTS seed_skills;
DROP TEMPORARY TABLE IF EXISTS seed_items;
DROP TEMPORARY TABLE IF EXISTS seed_numbers;
DROP TEMPORARY TABLE IF EXISTS seed_titles;
