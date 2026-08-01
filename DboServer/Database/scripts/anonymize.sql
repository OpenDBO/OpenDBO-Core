-- =====================================================================
-- anonymize.sql
--
-- PURPOSE:
--   Run this against a LOCAL COPY of a production database export
--   (never against production itself) to scrub sensitive/real user
--   data before using the dump for local debugging.
--
--   This replaces real password hashes, emails, and IPs with known
--   placeholder values, and grants every local account/character
--   admin + GM access so you can freely navigate and debug without
--   needing real user credentials.
--
-- WARNING:
--   Both UPDATE statements below have NO WHERE clause, meaning they
--   apply to EVERY row in the target table:
--     - accounts    (dbo_acc)   -> every account gets the placeholder
--                                  password/email and admin/GM flags
--     - characters  (dbo_char)  -> every character becomes GM level 10
--   This is intentional for a full anonymization pass over an entire
--   exported dataset. If you only want to anonymize a subset, add a
--   WHERE clause (e.g. WHERE account_id IN (...)).
--
--   Double-check your connection/target database before running this
--   file - it should only ever point at a local dev copy, never at
--   the production database itself.
-- =====================================================================

USE dbo_acc;

-- ---------------------------------------------------------------------
-- Scrub account credentials and status; grant local admin/GM access.
--
--  Password_hash / del_char_pw:
--      Replaced with the SHA3-256 hash of the known placeholder
--      password "password". This overwrites the real user's password
--      hash so it can no longer be used/cracked from the exported
--      data.
--
--  email:
--      Replaced with a placeholder test address so real user emails
--      don't linger in a local dev database.
--
--  acc_status:
--      'active' -> forces every account to a usable login state,
--      including accounts that were banned/suspended/disabled in
--      production. This is intentional: it lets any account from
--      the export (including previously banned ones) log in and be
--      used for debugging locally.
--
--  last_ip:
--      Replaced with loopback address (127.0.0.1) to scrub the real
--      last-known IP address.
--
--  mallpoints:
--      In-game cash-shop currency balance, set high for convenience
--      when debugging store/purchase flows locally.
--
--  admin:
--      1 -> flags the local account as a server administrator.
--
--  isGm:
--      10 -> Game Master permission level (scale depends on server
--      config; 10 typically = highest/owner-level GM access).
--
--  WaguCoins / EventCoins:
--      Additional in-game currencies, set high for local testing.
-- ---------------------------------------------------------------------
UPDATE accounts
SET Password_hash = 'c0067d4af4e87f00dbac63b6156828237059172d1bbeac67427345d6a9fda484', -- SHA3-256 hash of "password"
	email = "test@mail.com",
    del_char_pw = 'c0067d4af4e87f00dbac63b6156828237059172d1bbeac67427345d6a9fda484',    -- SHA3-256 hash of "password"
    acc_status = 'active',
    last_ip = '127.0.0.1',
	mallpoints = 10000000,
    admin = 1,
    isGm = 10,
    WaguCoins = 9999,
    EventCoins = 9999;


USE dbo_char;

-- ---------------------------------------------------------------------
-- Grant GM level 10 to every character in the anonymized local copy.
--
--  GameMaster:
--      10 -> in-game GM command/permission level for the character
--      (separate from the account-level isGm flag above; this is
--      what the game client/server checks for in-world GM commands).
--      Set on all characters so any character can be used to debug
--      GM-only commands/content locally.
-- ---------------------------------------------------------------------
UPDATE characters
SET GameMaster = 10;