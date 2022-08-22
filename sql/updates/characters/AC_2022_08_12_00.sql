-- run on character db
ALTER TABLE `daily_players_reports`
    ADD COLUMN `no_fall_damage_reports` BIGINT UNSIGNED NOT NULL DEFAULT 0 AFTER `antiknockback_reports`;
    
ALTER TABLE `players_reports_status`
    ADD COLUMN `no_fall_damage_reports` BIGINT UNSIGNED NOT NULL DEFAULT 0 AFTER `antiknockback_reports`;
