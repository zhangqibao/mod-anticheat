-- run on Characters DB
ALTER TABLE `daily_players_reports`
	ADD COLUMN `op_ack_hack_reports` BIGINT UNSIGNED NOT NULL DEFAULT 0 AFTER `no_fall_damage_reports`;

ALTER TABLE `players_reports_status`
	ADD COLUMN `op_ack_hack_reports` BIGINT UNSIGNED NOT NULL DEFAULT 0 AFTER `no_fall_damage_reports`;
