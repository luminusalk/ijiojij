/****************************************************************************!
*                _           _   _   _                                       *
*               | |__  _ __ / \ | |_| |__   ___ _ __   __ _                  *
*               | '_ \| '__/ _ \| __| '_ \ / _ \ '_ \ / _` |                 *
*               | |_) | | / ___ \ |_| | | |  __/ | | | (_| |                 *
*               |_.__/|_|/_/   \_\__|_| |_|\___|_| |_|\__,_|                 *
*                                                                            *
*                            www.brathena.org                                *
******************************************************************************
* src/map/battle.h                                                           *
******************************************************************************
* Copyright (c) brAthena Dev Team                                            *
* Copyright (c) Hercules Dev Team                                            *
* Copyright (c) Athena Dev Teams                                             *
*                                                                            *
* Licenciado sob a licença GNU GPL                                           *
* Para mais informações leia o arquivo LICENSE na raíz do emulador           *
*****************************************************************************/

#ifndef MAP_BATTLE_H
#define MAP_BATTLE_H

#include "map/map.h" //ELE_MAX
#include "common/cbasetypes.h"

/**
 * Declarations
 **/
struct map_session_data;
struct mob_data;
struct block_list;
struct weapon_atk;
struct status_change;
struct status_data;

/**
 * Defines
 **/
#define MIN_HAIR_STYLE  (battle->bc->min_hair_style)
#define MAX_HAIR_STYLE  (battle->bc->max_hair_style)
#define MIN_HAIR_COLOR  (battle->bc->min_hair_color)
#define MAX_HAIR_COLOR  (battle->bc->max_hair_color)
#define MIN_CLOTH_COLOR (battle->bc->min_cloth_color)
#define MAX_CLOTH_COLOR (battle->bc->max_cloth_color)
#define MIN_BODY_STYLE (battle->bc->min_body_style)
#define MAX_BODY_STYLE (battle->bc->max_body_style)

#define is_boss(bl)     (status_get_mode(bl)&MD_BOSS) // Can refine later [Aru]

/**
 * Enumerations
 **/

enum {
	// Flag of the final calculation
	BF_NONE       = 0x0000,
	BF_WEAPON     = 0x0001,
	BF_MAGIC      = 0x0002,
	BF_MISC       = 0x0004,
	BF_SHORT      = 0x0010,
	BF_LONG       = 0x0040,
	BF_SKILL      = 0x0100,
	BF_NORMAL     = 0x0200,
	BF_WEAPONMASK = 0x000f,
	BF_RANGEMASK  = 0x00f0,
	BF_SKILLMASK  = 0x0f00,
};

// state of a single attack attempt; used in flee/def penalty calculations when mobbed
typedef enum damage_lv {
	ATK_NONE,    // not an attack
	ATK_LUCKY,   // attack was lucky-dodged
	ATK_FLEE,    // attack was dodged
	ATK_MISS,    // attack missed because of element/race modifier.
	ATK_BLOCK,   // attack was blocked by some skills.
	ATK_DEF      // attack connected
} damage_lv;

enum e_battle_check_target { //New definitions [Skotlex]
	BCT_NOONE       =   0x000000,
	BCT_SELF        =   0x010000,
	BCT_ENEMY       =   0x020000,
	BCT_PARTY       =   0x040000,
	BCT_GUILDALLY   =   0x080000, ///< Only allies, NOT guildmates
	BCT_NEUTRAL     =   0x100000,
	BCT_SAMEGUILD   =   0x200000, ///< No Guild Allies

	BCT_GUILD       =   0x280000, ///< Guild AND allies (BCT_SAMEGUILD|BCT_GUILDALLY)

	BCT_NOGUILD     =   0x170000, ///< This must be (~BCT_GUILD&BCT_ALL)
	BCT_NOPARTY     =   0x3b0000, ///< This must be (~BCT_PARTY&BCT_ALL)
	BCT_NOENEMY     =   0x3d0000, ///< This must be (~BCT_ENEMY&BCT_ALL)

	BCT_ALL         =   0x3f0000, ///< Sum of BCT_NOONE to BCT_SAMEGUILD
};

/**
 * Values used by (struct Damage).type, as well as clif->damage(type) and clif->skill_damage(type)
 *
 * Note: some values may not apply in some contexts.
 */
enum battle_dmg_type {
	BDT_NORMAL      = 0,  // Normal attack
	//BDT_PICKUP      = 1,  // Pick up item
	//BDT_SITDOWN     = 2,  // Sit down
	//BDT_STANDUP     = 3,  // Stand up
	BDT_ENDURE      = 4,  // Damage (endure)
	BDT_SPLASH      = 5,  // Splash
	BDT_SKILL       = 6,  // Skill
	//BDT_REPEAT      = 7,  // (repeat damage?)
	BDT_MULTIHIT    = 8,  // Multi-hit damage
	BDT_MULTIENDURE = 9,  // Multi-hit damage (endure)
	BDT_CRIT        = 10, // Critical hit
	BDT_PDODGE      = 11, // Lucky dodge
	//BDT_TOUCH       = 12, // (touch skill?)
};

/**
 * Structures
 **/

// damage structure
struct Damage {
	int64 damage,damage2; //right, left dmg
	int type,div_; //chk clif_damage for type @TODO add an enum ? ;  nb of hit
	int amotion,dmotion;
	int blewcount; //nb of knockback
	int flag; //chk BF_* flag, (enum below)
	enum damage_lv dmg_lv; //ATK_LUCKY,ATK_FLEE,ATK_DEF
};

struct Battle_Config {
	int warp_point_debug;
	int enable_critical;
	int mob_critical_rate;
	int critical_rate;
	int enable_baseatk;
	int enable_perfect_flee;
	int cast_rate, delay_rate;
	int delay_dependon_dex, delay_dependon_agi;
	int sdelay_attack_enable;
	int left_cardfix_to_right;
	int skill_add_range;
	int skill_out_range_consume;
	int skill_amotion_leniency;
	int skillrange_by_distance; //[Skotlex]
	int use_weapon_skill_range; //[Skotlex]
	int pc_damage_delay_rate;
	int defnotenemy;
	int vs_traps_bctall;
	int traps_setting;
	int summon_flora; //[Skotlex]
	int clear_unit_ondeath; //[Skotlex]
	int clear_unit_onwarp; //[Skotlex]
	int random_monster_checklv;
	int attr_recover;
	int item_auto_get;
	int flooritem_lifetime;
	int item_first_get_time;
	int item_second_get_time;
	int item_third_get_time;
	int mvp_item_first_get_time;
	int mvp_item_second_get_time;
	int mvp_item_third_get_time;
	int base_exp_rate,job_exp_rate;
	int drop_rate0item;
	int death_penalty_type;
	int death_penalty_base,death_penalty_job;
	int pvp_exp;  // [MouseJstr]
	int gtb_sc_immunity;
	int zeny_penalty;
	int restart_hp_rate;
	int restart_sp_rate;
	int mvp_exp_rate;
	int mvp_hp_rate;
	int monster_hp_rate;
	int monster_max_aspd;
	int view_range_rate;
	int chase_range_rate;
	int atc_spawn_quantity_limit;
	int atc_slave_clone_limit;
	int partial_name_scan;
	int skillfree;
	int skillup_limit;
	int wp_rate;
	int pp_rate;
	int monster_active_enable;
	int monster_damage_delay_rate;
	int monster_loot_type;
	int mob_skill_rate; //[Skotlex]
	int mob_skill_delay; //[Skotlex]
	int mob_count_rate;
	int no_spawn_on_player; //[Skotlex]
	int force_random_spawn; //[Skotlex]
	int mob_spawn_delay, plant_spawn_delay, boss_spawn_delay; // [Skotlex]
	int slaves_inherit_mode;
	int slaves_inherit_speed;
	int summons_trigger_autospells;
	int pc_walk_delay_rate; //Adjusts can't walk delay after being hit for players. [Skotlex]
	int walk_delay_rate; //Adjusts can't walk delay after being hit. [Skotlex]
	int multihit_delay;  //Adjusts can't walk delay per hit on multi-hitting skills. [Skotlex]
	int quest_skill_learn;
	int quest_skill_reset;
	int basic_skill_check;
	int guild_emperium_check;
	int guild_exp_limit;
	int guild_max_castles;
	int guild_skill_relog_delay;
	int emergency_call;
	int guild_aura;
	int pc_invincible_time;

	int pet_catch_rate_official_formula;
	int pet_catch_rate;
	int pet_rename;
	int pet_friendly_rate;
	int pet_hungry_delay_rate;
	int pet_status_support;
	int pet_attack_support;
	int pet_damage_support;
	int pet_support_min_friendly; //[Skotlex]
	int pet_support_rate;
	int pet_attack_exp_to_master;
	int pet_attack_exp_rate;
	int pet_lv_rate; //[Skotlex]
	int pet_max_stats; //[Skotlex]
	int pet_max_atk1; //[Skotlex]
	int pet_max_atk2; //[Skotlex]
	int pet_no_gvg; //Disables pets in gvg. [Skotlex]
	int pet_no_pvp; //Desabilitar animais de estimação em pvp. [pedrodks]
	int pet_no_fvf; //Desabilitar animais de estimação em fvf. [pedrodks]
	int pet_equip_required;

	int skill_min_damage;
	int finger_offensive_type;
	int heal_exp;
	int max_heal_lv;
	int max_heal; //Mitternacht
	int resurrection_exp;
	int shop_exp;
	int combo_delay_rate;
	int item_check;
	int item_use_interval; //[Skotlex]
	int cashfood_use_interval;
	int wedding_modifydisplay;
	int wedding_ignorepalette; //[Skotlex]
	int xmas_ignorepalette; // [Valaris]
	int summer_ignorepalette; // [Zephyrus]
	int hanbok_ignorepalette;
	int oktoberfest_ignorepalette; //[Pedrodks]
	int summer2_ignorepalette; //[Pedrodks]
	int natural_healhp_interval;
	int natural_healsp_interval;
	int natural_heal_skill_interval;
	int natural_heal_weight_rate;
	int arrow_decrement;
	int max_aspd;
	int max_walk_speed; //Maximum walking speed after buffs [Skotlex]
	int max_hp;
	int max_sp;
	int aura_lv;
	int max_parameter, max_baby_parameter;
	int max_cart_weight;
	int skill_log;
	int battle_log;
	int etc_log;
	int save_clothcolor;
	int undead_detect_type;
	int auto_counter_type;
	int min_hitrate; //[Skotlex]
	int max_hitrate; //[Skotlex]
	int agi_penalty_target;
	int agi_penalty_type;
	int agi_penalty_count;
	int agi_penalty_num;
	int vit_penalty_target;
	int vit_penalty_type;
	int vit_penalty_count;
	int vit_penalty_num;
	int weapon_defense_type;
	int magic_defense_type;
	int skill_reiteration;
	int skill_nofootset;
	int pc_cloak_check_type;
	int monster_cloak_check_type;
	int estimation_type;
	int gvg_flee_penalty;
	int mob_changetarget_byskill;
	int attack_direction_change;
	int land_skill_limit;
	int monster_class_change_recover;
	int produce_item_name_input;
	int display_skill_fail;
	int chat_warpportal;
	int mob_warp;
	int dead_branch_active;
	int vending_max_value;
	int vending_over_max;
	int vending_tax;
	int show_steal_in_same_party;
	int party_share_type;
	int party_hp_mode;
	int party_change_leader_same_map;
	int party_show_share_picker;
	int show_picker_item_type;
	int attack_attr_none;
	int item_rate_mvp, item_rate_common, item_rate_common_boss, item_rate_card, item_rate_card_boss,
	item_rate_equip, item_rate_equip_boss, item_rate_heal, item_rate_heal_boss, item_rate_use,
	item_rate_use_boss, item_rate_treasure, item_rate_adddrop;

	int logarithmic_drops;
	int item_drop_common_min,item_drop_common_max; // Added by TyrNemesis^
	int item_drop_card_min,item_drop_card_max;
	int item_drop_equip_min,item_drop_equip_max;
	int item_drop_mvp_min,item_drop_mvp_max; // End Addition
	int item_drop_heal_min,item_drop_heal_max; // Added by Valatris
	int item_drop_use_min,item_drop_use_max; //End
	int item_drop_treasure_min,item_drop_treasure_max; //by [Skotlex]
	int item_drop_adddrop_min,item_drop_adddrop_max; //[Skotlex]

	int prevent_logout; // Added by RoVeRT

	int alchemist_summon_reward; // [Valaris]
	int drops_by_luk;
	int drops_by_luk2;
	int equip_natural_break_rate; //Base Natural break rate for attacks.
	int equip_self_break_rate; //Natural & Penalty skills break rate
	int equip_skill_break_rate; //Offensive skills break rate
	int multi_level_up;
	int max_exp_gain_rate; //Max amount of exp bar % you can get in one go.
	int pk_mode;
	int pk_level_range;

	int manner_system; // end additions [Valaris]
	int show_mob_info;

	int gx_allhit;
	int gx_disptype;
	int devotion_level_difference;
	int player_skill_partner_check;
	int invite_request_check;
	int skill_removetrap_type;
	int disp_experience;
	int disp_zeny;
	int castle_defense_rate;
	int backstab_bow_penalty;
	int hp_rate;
	int sp_rate;
	int bone_drop;
	int buyer_name;
	int dancing_weaponswitch_fix;

	// eAthena additions
	int night_at_start; // added by [Yor]
	int day_duration; // added by [Yor]
	int night_duration; // added by [Yor]
	int ban_hack_trade; // added by [Yor]

	int min_hair_style; // added by [MouseJstr]
	int max_hair_style; // added by [MouseJstr]
	int min_hair_color; // added by [MouseJstr]
	int max_hair_color; // added by [MouseJstr]
	int min_cloth_color; // added by [MouseJstr]
	int max_cloth_color; // added by [MouseJstr]
	int pet_hair_style; // added by [Skotlex]

	int castrate_dex_scale; // added by [MouseJstr]
	int area_size; // added by [MouseJstr]
	int chat_area_size; // added by [gumi]
	int dead_area_size; // Monster die area [KirieZ]

	int max_def, over_def_bonus; //added by [Skotlex]

	int zeny_from_mobs; // [Valaris]
	int mobs_level_up; // [Valaris]
	int mobs_level_up_exp_rate; // [Valaris]
	int pk_min_level; // [celest]
	int skill_steal_max_tries; //max steal skill tries on a mob. if 0, then w/o limit [Lupus]
	int exp_calc_type;
	int exp_bonus_attacker;
	int exp_bonus_max_attacker;
	int min_skill_delay_limit;
	int default_walk_delay;
	int no_skill_delay;
	int attack_walk_delay;
	int require_glory_guild;
	int idle_no_share;
	int party_update_interval;
	int party_even_share_bonus;
	int delay_battle_damage;
	int hide_woe_damage;
	int display_version;

	int display_hallucination; // [Skotlex]
	int use_statpoint_table; // [Skotlex]

	int ignore_items_gender; //[Lupus]

	int copyskill_restrict; // [Aru]
	int berserk_cancels_buffs; // [Aru]
	int mob_ai; //Configures various mob_ai settings to make them smarter or dumber(official). [Skotlex]
	int hom_setting; //Configures various homunc settings which make them behave unlike normal characters.. [Skotlex]
	int dynamic_mobs; // Dynamic Mobs [Wizputer] - battle.conf flag implemented by [random]
	int mob_remove_damaged; // Dynamic Mobs - Remove mobs even if damaged [Wizputer]
	int mob_remove_delay; // Dynamic Mobs - delay before removing mobs from a map [Skotlex]
	int mob_active_time; //Duration through which mobs execute their Hard AI after players leave their area of sight.
	int boss_active_time;

	int show_hp_sp_drain, show_hp_sp_gain; //[Skotlex]
	int show_katar_crit_bonus;

	int mob_npc_event_type; //Determines on who the npc_event is executed. [Skotlex]

	int character_size; // if riders have size=2, and baby class riders size=1 [Lupus]
//	int rare_drop_announce; // chance <= to show rare drops global announces

	int retaliate_to_master; //Whether when a mob is attacked by another mob, it will retaliate versus the mob or the mob's master. [Skotlex]

	int duel_allow_pvp; // [LuzZza]
	int duel_allow_gvg; // [LuzZza]
	int duel_allow_teleport; // [LuzZza]
	int duel_autoleave_when_die; // [LuzZza]
	int duel_time_interval; // [LuzZza]
	int duel_only_on_same_map; // [Toms]

	int skip_teleport_lv1_menu; // possibility to disable (skip) Teleport Lv1 menu, that have only two lines `Random` and `Cancel` [LuzZza]
	int mob_max_skilllvl;
	int allow_skill_without_day; // [Komurka]
	int allow_es_magic_pc; // [Skotlex]
	int skill_wall_check; // [Skotlex]
	int official_cell_stack_limit; // [Playtester]
	int custom_cell_stack_limit; // [Skotlex]
	int check_occupied_cells; // [4144]
	int skill_caster_check; // [Skotlex]
	int sc_castcancel; // [Skotlex]
	int pc_sc_def_rate; // [Skotlex]
	int mob_sc_def_rate;
	int pc_max_sc_def;
	int mob_max_sc_def;
	int enable_luk_influence, pc_luk_sc_def, mob_luk_sc_def;
	
	int sg_angel_skill_ratio;
	int sg_miracle_skill_ratio;
	int sg_miracle_skill_duration;
	int autospell_stacking; //Enables autospell cards to stack. [Skotlex]
	int override_mob_names; //Enables overriding spawn mob names with the mob_db names. [Skotlex]
	int min_chat_delay; //Minimum time between client messages. [Skotlex]
	int friend_auto_add; //When accepting friends, both get friended. [Skotlex]
	int hvan_explosion_intimate; // fix [albator]
	int hom_rename;
	int homunculus_show_growth; //[orn]
	int homunculus_friendly_rate;
	int quest_exp_rate;
	int autotrade_mapflag;
	int at_timeout;
	int homunculus_autoloot;
	int idle_no_autoloot;
	int max_guild_alliance;
	int ksprotection;
	int auction_feeperhour;
	int auction_maximumprice;
	int homunculus_auto_vapor; //Keep Homunculus from Vaporizing when master dies. [L0ne_W0lf]
	int display_status_timers; //Show or hide skill buff/delay timers in recent clients [Sara]
	int skill_add_heal_rate; //skills that bHealPower has effect on [Inkfish]
	int eq_single_target_reflectable;
	int invincible_nodamage;
	int mob_slave_keep_target;
	int autospell_check_range; //Enable range check for autospell bonus. [L0ne_W0lf]
	int knockback_left;
	int client_reshuffle_dice;  // Reshuffle /dice
	int client_sort_storage;
	int feature_buying_store;
	int feature_search_stores;
	int searchstore_querydelay;
	int searchstore_maxresults;
	int display_party_name;
	int cashshop_show_points;
	int mail_show_status;
	int client_limit_unit_lv;
	int client_emblem_max_blank_percent;

	// [BattleGround Settings]
	int bg_update_interval;
	int bg_flee_penalty;

	// rAthena
	int max_third_parameter;
	int max_baby_third_parameter;
	int max_extended_parameter;
	int max_summoner_parameter;
	int mvp_exp_reward_message;
	int mob_eye_range_bonus; //Vulture's Eye and Snake's Eye range bonus
	int prevent_logout_trigger;
	int boarding_halter_speed;
	int atcommand_max_stat_bypass;
	int max_third_aspd;
	int vcast_stat_scale;

	int mvp_tomb_enabled;
	int mvp_tomb_spawn_delay;

	int atcommand_suggestions_enabled;
	int min_npc_vendchat_distance;
	int vendchat_near_hiddennpc;
	int atcommand_mobinfo_type;

	int mob_size_influence; // Enable modifications on earned experience, drop rates and monster status depending on monster size. [mkbu95]
	int bowling_bash_area;
	int mob_chase_refresh; //How often a monster should refresh its chase [Playtester]
	int mob_icewall_walk_block; //How a normal monster should be trapped in icewall [Playtester]
	int boss_icewall_walk_block; //How a boss monster should be trapped in icewall [Playtester]

	/** Hercules **/
	int skill_trap_type;
	int trap_reflect;
	int item_restricted_consumption_type;
	int unequip_restricted_equipment;
	int max_walk_path;
	int item_enabled_npc;
	int packet_obfuscation;
	int idletime_criteria;
	int gm_ignore_warpable_area;

	int client_accept_chatdori; // [Ai4rei/Mirei]
	int snovice_call_type;
	int guild_notice_changemap;

	int feature_banking;
	int feature_auction;

	int mon_trans_disable_in_gvg;

	int case_sensitive_aegisnames;
	int guild_castle_invite;
	int guild_castle_expulsion;

	int song_timer_reset; // [csnv]
	int snap_dodge; // Enable or disable dodging damage snapping away [csnv]
	int stormgust_knockback;

	int feature_roulette;

	int show_monster_hp_bar; //[pedrodks]

	int all_hp_monster_map; //[pedrodks]

	int fix_warp_hit_delay_abuse;

	// Refine Def/Atk
	int costume_refine_def, shadow_refine_def;
	int shadow_refine_atk;

	// BodyStyle
	int min_body_style, max_body_style;
	int save_body_style;

	// Warp Face Direction
	int player_warp_keep_direction;

	int atcommand_levelup_events;	// Enable atcommands trigger level up events for NPCs

	int bow_unequip_arrow;

	// Costume System [Mhalicot/Rebel/Zephyrus]
	int reserved_costume_id;
	
	int dispel_song; //Can songs be dispelled?

	/** brAthena **/
	// Configuração para a habilidade de revestimento total dos criadores. [CarlosHenrq]
	int creator_fullprotect;
	// Configuração para bloquear jogadores de abrir chat/vendas na mesma celula. [CarlosHenrq]
	int vending_chat_block_same_cell;

	// Configuração para bloquear jogadores de abrir chat/loja próximos uns aos outros. [CarlosHenrq]
	int vending_chat_block_range_check_cells;
	int vending_chat_block_range_count_players;

	int get_refine; // Block Refine - [Orce]
	
	// Configuração para habilidades de remover armas. [Sir Will]
	int strip_weapon;

	// Configuração para impedir drop de itens dentro de mapas com o flag 'town'. [CarlosHenrq]
	int nodrop_in_town;

	// Configuração para o storage do personagem se ajustar automaticamente
	// quando o limite de itens máximo for atingido. [CarlosHenrq]
	int storage_auto_new_entry;	

	// [CreativeSD]: Queue System
	int queue_join_delay;
	int queue_only_towns;
	int queue_notify;

	// [CreativeSD] BattleGround Queue Expansive System
	int bg_mode;
	int bg_timer_attack;
	int bg_group_filter;
	int bg_achievement;
	int bg_player_announce;
	int bg_afk_timer;
	int bg_afk_team_report;
	int bg_afk_warp_save_point;
	int bg_ranking_rows;
	int bg_enable_skills;
	int bg_name_position;
	
	/**
	* Extended Vending system [Lilith]
	**/
	int extended_vending;
	int show_broadcas_info;
	int show_item_vending;
	int ex_vending_info;
	int item_zeny;
	int item_cash;
	int item_universe;

	int feature_rodex;
	int feature_rodex_use_accountmail;

	//homunc e pet autofeed [pedrodks]
	int feature_enable_homun_autofeed;
	int feature_enable_pet_autofeed;

	//Nao usar item com storage aberto [pedrodks]
	int storage_use_item;

	//Sistema de Atendimento [pedrodks]
	int feature_enable_attendance_system;
	int feature_attendance_endtime;

	int min_item_buy_price;
	int min_item_sell_price;

	int display_fake_hp_when_dead;

	//Switch system [pedrodks]
	int feature_equipswitch;

	//monster drop radom options [pedrodks]
	int option_drop_max_loop;

	//bloquear teleporte com cordenadas em mapas mvp's [pedrodks]
	int warp_block_coordinates_map_mvp;

	//desativar a imunidade da gtb a dispell [pedrodks]
	int desactive_gtb_imunity_dispell;

	//mobsearch com mini pontos no mapa ou não [pedrodks]
	int mobsearch_minimap_point;

	//configuração do emblema do sistema de facção [pedrodks]
	int emblem_fac_sys;

	//anti macro [pedrodks]
	int action_keyboard_limit;
	int action_keyboard_limit_item;
	int action_mouse_limit;
	int action_dual_limit;
	int status_min_duration;
	int hansip_protection; //Make sure nodelay grf user still follow the rules [Kichi]
	int hansip_min_delay;
	int hansip_flood_protection;
	int hansip_spam_count;
	int hansip_spam_punish;

	// punição macro [pedrodks]
	int macropunit;

	//mapa pvp liberado para usar pets [pedrodks]
	int active_mappet;

	//sistema de refino de asas [pedrodks]
	int enable_refinery_ui;
	int replace_refine_npcs;
	int change_cash_to_refine;
	int block_weapons_refine;
	int id_item_refine_apartir;

	//edição no proteger terreno [pedrodks]
	int active_meteor_animation;
	int active_wall_protect_land;
	int active_pneuma_protect_land;

	//[pedrodks]
	//configuração de congelamento e petrificação
	int ero_max_status;
	int ero_cong_formula;
	int ero_petri_formula;
	int ero_cong_formula_luk;
	int ero_cong_formula_mdef;
	int ero_petri_formula_luk;
	int ero_petri_formula_mdef;
	int ero_cong_formula_fix;
	int ero_petri_formula_fix;
	int ero_cong_des_t2;
	int ero_petri_des_t2;
	int ero_max_luk_cong;
	int ero_max_luk_petri;
	int ero_max_divi;
	int ero_mdef_divi;
	int ero_mdef2_divi;
	int ero_fs_mdef;

	// MvP and Mini-Boss Card Ad and Log System [Bad]
	int announcement_and_log_system;
	int mvp_card_announce_system;
	int mvp_card_announce;
	int dropped_mvp_card_log;
	int set_drop_announce_color;
	int mini_boss_card_announce_system;
	int mini_boss_card_announce;
	int dropped_mini_boss_card_log;
	int set_drop_card_announce_color;

	//pedrodks
	//SISTEMA DE SKILLS BATTLE OF RENEGADES
	double tsukuyomi_rate_luk;
	double korishinchu_rate_luk;
	double kokuangyo_rate_luk;
	double kanenooto_rate_luk;
	double genwaku_rate_luk;
	double shibari_rate_luk;
	double fchakra_rate_luk;
	double nehanshoja_rate_luk;
	double narakumi_rate_luk;

	//kuchyoses
	int kuchy_cach_id01;
	int kuchy_cach_id02;
	int kuchy_cach_id03;
	int kuchy_cobr_id01;
	int kuchy_cobr_id02;
	int kuchy_cobr_id03;
	int kuchy_lesm_id01;
	int kuchy_lesm_id02;
	int kuchy_lesm_id03;
	int kuchy_maca_id01;
	int kuchy_maca_id02;
	int kuchy_maca_id03;
	int kuchy_sapo_id01;
	int kuchy_sapo_id02;
	int kuchy_sapo_id03;
	int kuchy_ibus_id01;
	int kuchy_ibus_id02;
	int kuchy_ibus_id03;
	int kuchy_tayu_id01;
	int kuchy_tayu_id02;
	int kuchy_tayu_id03;
	int kuchy_kugu_id01;
	int kuchy_kugu_id02;
	int kuchy_kugu_id03;

	int byak_control;
	int prem_diar_vip;

	//katon
	int katon_01_active;
	int katon_02_active;
	int katon_03_active;
	int katon_04_active;
	int katon_05_active;
	int katon_01_chance;
	int katon_02_chance;
	int katon_03_chance;
	int katon_04_chance;
	int katon_05_chance;
	int suiton_01_active;
	int suiton_02_active;
	int suiton_03_active;
	int suiton_04_active;
	int suiton_05_active;
	int suiton_01_chance;
	int suiton_02_chance;
	int suiton_03_chance;
	int suiton_04_chance;
	int suiton_05_chance;
	int raiton_01_active;
	int raiton_02_active;
	int raiton_03_active;
	int raiton_04_active;
	int raiton_05_active;
	int raiton_06_active;
	int raiton_07_active;
	int raiton_08_active;
	int raiton_01_chance;
	int raiton_02_chance;
	int raiton_03_chance;
	int raiton_04_chance;
	int raiton_05_chance;
	int raiton_06_chance;
	int raiton_07_chance;
	int raiton_08_chance;
	int doton_01_active;
	int doton_01_chance;
	int doton_02_active;
	int doton_02_chance;
	double queimadura_balance;
	double cegueira_balance;
	double hipotermia_balance;

	//sistema de evolução
	int doujutsu_sharigan_item_nv1;
	int doujutsu_sharigan_item_nv2;
	int doujutsu_sharigan_item_nv3;
	int doujutsu_sharigan_item_nv4;
	int doujutsu_byakugan_item_nv1;
	int doujutsu_byakugan_item_nv2;
	int doujutsu_byakugan_item_nv3;
	int doujutsu_byakugan_item_nv4;
	int hidden_hachimon_item_nv1;
	int hidden_hachimon_item_nv2;
	int hidden_hachimon_item_nv3;
	int hidden_hachimon_item_nv4;
	int hidden_baika_item_nv1;
	int hidden_baika_item_nv2;
	int hidden_baika_item_nv3;
	int hidden_baika_item_nv4;
	int hidden_sombras_item_nv1;
	int hidden_sombras_item_nv2;
	int hidden_sombras_item_nv3;
	int hidden_sombras_item_nv4;
	int kg_hyoton_item_nv1;
	int kg_hyoton_item_nv2;
	int kg_hyoton_item_nv3;
	int kg_hyoton_item_nv4;
	int refine_01_exp_nv_1;
	int refine_02_exp_nv_1;
	int refine_03_exp_nv_1;
	int refine_04_exp_nv_1;
	int refine_05_exp_nv_1;
	int refine_06_exp_nv_1;
	int refine_07_exp_nv_1;
	int refine_08_exp_nv_1;
	int refine_09_exp_nv_1;
	int refine_10_exp_nv_1;
	int refine_01_exp_nv_2;
	int refine_02_exp_nv_2;
	int refine_03_exp_nv_2;
	int refine_04_exp_nv_2;
	int refine_05_exp_nv_2;
	int refine_06_exp_nv_2;
	int refine_07_exp_nv_2;
	int refine_08_exp_nv_2;
	int refine_09_exp_nv_2;
	int refine_10_exp_nv_2;
	int refine_01_exp_nv_3;
	int refine_02_exp_nv_3;
	int refine_03_exp_nv_3;
	int refine_04_exp_nv_3;
	int refine_05_exp_nv_3;
	int refine_06_exp_nv_3;
	int refine_07_exp_nv_3;
	int refine_08_exp_nv_3;
	int refine_09_exp_nv_3;
	int refine_10_exp_nv_3;
	int refine_01_exp_nv_4;
	int refine_02_exp_nv_4;
	int refine_03_exp_nv_4;
	int refine_04_exp_nv_4;
	int refine_05_exp_nv_4;
	int refine_06_exp_nv_4;
	int refine_07_exp_nv_4;
	int refine_08_exp_nv_4;
	int refine_09_exp_nv_4;
	int refine_10_exp_nv_4;
	int refine_item_protect;
	int refine_01_item_protect_quant;
	int refine_02_item_protect_quant;
	int refine_03_item_protect_quant;
	int refine_04_item_protect_quant;
	int refine_05_item_protect_quant;
	int refine_06_item_protect_quant;
	int refine_07_item_protect_quant;
	int refine_08_item_protect_quant;
	int refine_09_item_protect_quant;
	int refine_10_item_protect_quant;
	int refine_01_value;
	int refine_02_value;
	int refine_03_value;
	int refine_04_value;
	int refine_05_value;
	int refine_06_value;
	int refine_07_value;
	int refine_08_value;
	int refine_09_value;
	int refine_10_value;

	//sistema de sangramento
	int sang_mode;
	double sang_hp;
	int sang_time;

	int lotus_mode;
	int lotus_desactive;

	int lotus_4_chance;
	int lotus_8_chance;
	int lotus_16_chance;
	int lotus_32_chance;
	int lotus_64_chance;
	int lotus_128_chance;

	int lotus_4_type;
	int lotus_8_type;
	int lotus_16_type;
	int lotus_32_type;
	int lotus_64_type;
	int lotus_128_type;

	int lotus_64_coma;
	int lotus_128_coma;

	int gama_01_hit_chance;
	int gama_02_hit_chance;
	int hebi_01_hit_chance;
	int hebi_02_hit_chance;

	int gp01_chance;

	int gp02_chance;
	int gp02_time;

	int hp01_chance;
	int hp01_time;

	int hp02_chance;
	int hp02_time;
	int hp02_agi_rate;
	int hp02_agi_time;

	int bor_atk_base_limit;

	int chibaku_chance;

	double gen_01_chance;
	double gen_02_chance;
	double gen_03_chance;

	int amat_fogo_quant;

	int nara_01_block;
	int nara_02_block;
	int nara_03_block;
	int nara_03_block_mob;

	int ranked_system_ativado;
	int point_rank_e;
	int point_rank_d;
	int point_rank_c;
	int point_rank_b;
	int point_rank_a;
	int point_rank_s;

	int rank_e_mata_e;
	int rank_e_mata_d;
	int rank_e_mata_c;
	int rank_e_mata_b;
	int rank_e_mata_a;
	int rank_e_mata_s;
	int rank_e_morre_e;
	int rank_e_morre_d;
	int rank_e_morre_c;
	int rank_e_morre_b;
	int rank_e_morre_a;
	int rank_e_morre_s;
	int rank_d_mata_e;
	int rank_d_mata_d;
	int rank_d_mata_c;
	int rank_d_mata_b;
	int rank_d_mata_a;
	int rank_d_mata_s;
	int rank_d_morre_e;
	int rank_d_morre_d;
	int rank_d_morre_c;
	int rank_d_morre_b;
	int rank_d_morre_a;
	int rank_d_morre_s;
	int rank_c_mata_e;
	int rank_c_mata_d;
	int rank_c_mata_c;
	int rank_c_mata_b;
	int rank_c_mata_a;
	int rank_c_mata_s;
	int rank_c_morre_e;
	int rank_c_morre_d;
	int rank_c_morre_c;
	int rank_c_morre_b;
	int rank_c_morre_a;
	int rank_c_morre_s;
	int rank_b_mata_e;
	int rank_b_mata_d;
	int rank_b_mata_c;
	int rank_b_mata_b;
	int rank_b_mata_a;
	int rank_b_mata_s;
	int rank_b_morre_e;
	int rank_b_morre_d;
	int rank_b_morre_c;
	int rank_b_morre_b;
	int rank_b_morre_a;
	int rank_b_morre_s;
	int rank_a_mata_e;
	int rank_a_mata_d;
	int rank_a_mata_c;
	int rank_a_mata_b;
	int rank_a_mata_a;
	int rank_a_mata_s;
	int rank_a_morre_e;
	int rank_a_morre_d;
	int rank_a_morre_c;
	int rank_a_morre_b;
	int rank_a_morre_a;
	int rank_a_morre_s;
	int rank_s_mata_e;
	int rank_s_mata_d;
	int rank_s_mata_c;
	int rank_s_mata_b;
	int rank_s_mata_a;
	int rank_s_mata_s;
	int rank_s_morre_e;
	int rank_s_morre_d;
	int rank_s_morre_c;
	int rank_s_morre_b;
	int rank_s_morre_a;
	int rank_s_morre_s;

	int weapon_exp_lv_point[9];

	int doton_01_hateffect_area;
	int doton_01_hateffectid;

	int doton_02_hateffect_area;
	int doton_02_hateffectid;

	int bijuu_01_id;
	int bijuu_02_id;
	int bijuu_03_id;
	int bijuu_04_id;
	int bijuu_05_id;
	int bijuu_06_id;
	int bijuu_07_id;
	int bijuu_08_id;
	int bijuu_09_id;

	int bijuu_01_item;
	int bijuu_02_item;
	int bijuu_03_item;
	int bijuu_04_item;
	int bijuu_05_item;
	int bijuu_06_item;
	int bijuu_07_item;
	int bijuu_08_item;
	int bijuu_09_item;

	int bijuu_01_chance;
	int bijuu_02_chance;
	int bijuu_03_chance;
	int bijuu_04_chance;
	int bijuu_05_chance;
	int bijuu_06_chance;
	int bijuu_07_chance;
	int bijuu_08_chance;
	int bijuu_09_chance;

	double bijuu_vida;

	//delay das habilidades do anti-macro definidas no arquivo delay.conf [pedrodks]
	//Espadachim
	int d_SM_BASH;
	int d_SM_MAGNUM;

	//Mago
	int d_MG_NAPALMBEAT;
	int d_MG_SOULSTRIKE;
	int d_MG_FROSTDIVER;
	int d_MG_STONECURSE;
	int d_MG_FIREBALL;
	int d_MG_FIREWALL;
	int d_MG_THUNDERSTORM;
	int d_MG_COLDBOLT;
	int d_MG_FIREBOLT;
	int d_MG_LIGHTNINGBOLT;

	//noviço
	int d_AL_DECAGI;
	int d_AL_CRUCIS;
	int d_AL_HOLYLIGHT;

	//Mercador
	int d_MC_MAMMONITE;
	int d_MC_CARTREVOLUTION;

	//Arqueiro
	int d_AC_DOUBLE;
	int d_AC_SHOWER;
	int d_AC_CHARGEARROW;

	//Gatuno
	int d_TF_POISON;
	int d_TF_BACKSLIDING;

	//Cavaleiro
	int d_KN_PIERCE;
	int d_KN_BRANDISHSPEAR;
	int d_KN_SPEARSTAB;
	int d_KN_SPEARBOOMERANG;
	int d_KN_CHARGEATK;
	int d_KN_BOWLINGBASH;

	//Sacerdote
	int d_PR_LEXDIVINA;
	int d_PR_TURNUNDEAD;
	int d_PR_LEXAETERNA;
	int d_PR_MAGNUS;
	int d_PR_REDEMPTIO;

	//bruxo
	int d_WZ_FIREPILLAR;
	int d_WZ_SIGHTRASHER;
	int d_WZ_FIREIVY;
	int d_WZ_METEOR;
	int d_WZ_VERMILION;
	int d_WZ_WATERBALL;
	int d_WZ_ICEWALL;
	int d_WZ_FROSTNOVA;
	int d_WZ_STORMGUST;
	int d_WZ_EARTHSPIKE;
	int d_WZ_HEAVENDRIVE;
	int d_WZ_QUAGMIRE;
	int d_WZ_ESTIMATION;
	int d_WZ_SIGHTBLASTER;
	int d_WZ_JUPITEL;

	//Ferreiro
	int d_BS_HAMMERFALL;

	//Caçador
	int d_HT_BLITZBEAT;
	int d_HT_POWER;
	int d_HT_PHANTASMIC;

	//Assasino
	int d_AS_GRIMTOOTH;
	int d_AS_VENOMKNIFE;
	int d_AS_SONICBLOW;

	//Arruaceiro
	int d_RG_BACKSTAP;
	int d_RG_RAID;
	int d_RG_GRAFFITI;
	int d_RG_FLAGGRAFFITI;
	int d_RG_COMPULSION;
	int d_RG_PLAGIARISM;
	int d_RG_CLOSECONFINE;

	//Alquimista
	int d_AM_DEMONSTRATION;
	int d_AM_ACIDTERROR;
	int d_AM_POTIONPITCHER;
	int d_AM_CANNIBALIZE;
	int d_AM_SPHEREMINE;
	int d_AM_FLAMECONTROL;
	int d_AM_DRILLMASTER;

	//Templário
	int d_CR_SHIELDBOOMERANG;
	int d_CR_HOLYCROSS;
	int d_CR_GRANDCROSS;

	//Monge
	int d_MO_CALLSPIRITS;
	int d_MO_ABSORBSPIRITS;
	int d_MO_INVESTIGATE;
	int d_MO_EXPLOSIONSPIRITS;
	int d_MO_EXTREMITYFIST;
	int d_MO_CHAINCOMBO;
	int d_MO_COMBOFINISH;
	int d_MO_KITRANSLATION;
	int d_MO_BALKYOUNG;
	int d_MO_FINGEROFFENSIVE;
	int d_MO_BODYRELOCATION;

	//Sábio
	int d_SA_CASTCANCEL;
	int d_SA_SPELLBREAKER;
	int d_SA_MONOCELL;
	int d_SA_CLASSCHANGE;
	int d_SA_SUMMONMONSTER;
	int d_SA_REVERSEORCISH;
	int d_SA_DEATH;
	int d_SA_FORTUNE;
	int d_SA_TAMINGMONSTER;
	int d_SA_QUESTION;
	int d_SA_GRAVITY;
	int d_SA_LEVELUP;
	int d_SA_INSTANTDEATH;
	int d_SA_FULLRECOVERY;
	int d_SA_COMA;
	int d_SA_DISPELL;

	//habilidades comum a bardo e dançarina
	int d_BD_ADAPTATION;
	int d_BD_ENCORE;
	int d_BD_LULLABY;
	int d_BD_RICHMANKIM;

	//bardo
	int d_BA_MUSICALSTRIKE;
	int d_BA_DISSONANCE;
	int d_BA_FROSTJOKER;
	int d_BA_WHISTLE;
	int d_BA_ASSASSINCROSS;
	int d_BA_POEMBRAGI;
	int d_BA_APPLEIDUN;
	int d_BA_PANGVOICE;

	//dançarina
	int d_DC_THROWARROW;
	int d_DC_UGLYDANCE;
	int d_DC_SCREAM;
	int d_DC_HUMMING;
	int d_DC_DONTFORGETME;
	int d_DC_FORTUNEKISS;
	int d_DC_SERVICEFORYOU;
	int d_DC_WINKCHARM;

	//Lorde
	int d_LK_SPIRALPIERCE;
	int d_LK_HEADCRUSH;
	int d_LK_JOINTBEAT;
	int d_LK_FURY;

	//arquimago
	int d_HW_MAGICCRASHER;
	int d_HW_NAPALMVULCAN;
	int d_HW_GANBANTEIN;
	int d_HW_GRAVITATION;

	//paladino
	int d_SM_AUTOBERSERK;
	int d_PA_SACRIFICE;
	int d_PA_PRESSURE;
	int d_PA_SHIELDCHAIN;

	//mestre
	int d_CH_PALMSTRIKE;
	int d_CH_TIGERFIST;
	int d_CH_CHAINCRUSH;
	int d_CH_SOULCOLLECT;

	//professor
	int d_PF_SOULCHANGE;
	int d_PF_SOULBURN;
	int d_PF_DOUBLECASTING;
	int d_PF_MINDBREAKER;
	int d_PF_SPIDERWEB;

	//Algoz
	int d_ASC_BREAKER;
	int d_ASC_METEORASSAULT;

	//Atirador de elite
	int d_SN_FALCONASSAULT;
	int d_SN_SHARPSHOOTING;

	//Criador
	int d_CR_ALCHEMY;
	int d_CR_SYNTHESISPOTION;
	int d_CR_SLIMPITCHER;
	int d_CR_FULLPROTECTION;
	int d_CR_ACIDDEMONSTRATION;

	//menestrel e cigana
	int d_CG_MOONLIT;
	int d_CG_MARIONETTE;
	int d_CG_HERMODE;
	int d_CG_TAROTCARD;
	int d_CG_ARROWVULCAN;

	//Desordeiro
	int d_ST_FULLSTRIP;

	//Sumo Sacerdote
	int d_HP_MANARECHARGE;

	//Mestre Ferreiro
	int d_WS_CARTTERMINATION;

	//taekwon
	int d_TK_STORMKICK;
	int d_TK_DOWNKICK;
	int d_TK_TURNKICK;
	int d_TK_JUMPKICK;
	int d_TK_POWER;
	int d_TK_HIGHJUMP;

	//Espiritualista
	int d_SL_KAIZEL;
	int d_SL_KAAHI;
	int d_SL_KAUPE;
	int d_SL_KAITE;
	int d_SL_KAINA;
	int d_SL_STIN;
	int d_SL_STUN;
	int d_SL_SMA;
	int d_SL_SWOO;
	int d_SL_SKE;
	int d_SL_SKA;
	int d_SL_HIGH;

	//Justiceiro
	int d_GS_TRIPLEACTION;
	int d_GS_BULLSEYE;
	int d_GS_MADNESSCANCEL;
	int d_GS_INCREASING;
	int d_GS_MAGICALBULLET;
	int d_GS_CRACKER;
	int d_GS_SINGLEACTION;
	int d_GS_CHAINACTION;
	int d_GS_TRACKING;
	int d_GS_DISARM;
	int d_GS_PIERCINGSHOT;
	int d_GS_RAPIDSHOWER;
	int d_GS_GATLINGFEVER;
	int d_GS_DUST;
	int d_GS_FULLBUSTER;
	int d_GS_SPREADATTACK;
	int d_GS_GROUNDDRIFT;
	int d_GS_DESPERADO;

	//Ninja
	int d_NJ_TOBIDOUGU;
	int d_NJ_SYURIKEN;
	int d_NJ_KUNAI;
	int d_NJ_HUUMA;
	int d_NJ_ZENYNAGE;
	int d_NJ_TATAMIGAESHI;
	int d_NJ_KASUMIKIRI;
	int d_NJ_SHADOWJUMP;
	int d_NJ_KIRIKAGE;
	int d_NJ_UTSUSEMI;
	int d_NJ_BUNSINJYUTSU;
	int d_NJ_NINPOU;
	int d_NJ_KOUENKA;
	int d_NJ_KAENSIN;
	int d_NJ_BAKUENRYU;
	int d_NJ_SUITON;
	int d_NJ_HYOUSYOURAKU;
	int d_NJ_HUUJIN;
	int d_NJ_RAIGEKISAI;
	int d_NJ_KAMAITACHI;
	int d_NJ_NEN;
	int d_NJ_ISSEN;
	int d_NJ_HYOUSENSOU;
	int d_BA_BAIKANOJUTSU;
	int d_BA_PVERDE;
	int d_BA_PAMARELA;
	int d_BA_PVERMELHA;
	int d_BOR_CHOUJI_01;
	int d_PC_PORTAO01;
	int d_PC_PORTAO02;
	int d_PC_PORTAO03;
	int d_PC_PORTAO04;
	int d_PC_PORTAO05;
	int d_PC_PORTAO06;
	int d_PC_PORTAO07;
	int d_PC_PORTAO08;
	int d_BOR_KUCHYCACH01;
	int d_BOR_KUCHYCACH02;
	int d_BOR_KUCHYCACH03;
	int d_BOR_KUCHYCOBRA01;
	int d_BOR_KUCHYCOBRA02;
	int d_BOR_KUCHYCOBRA03;
	int d_BOR_KUCHYLESMA01;
	int d_BOR_KUCHYLESMA02;
	int d_BOR_KUCHYLESMA03;
	int d_BOR_KUCHYMACAC01;
	int d_BOR_KUCHYMACAC02;
	int d_BOR_KUCHYMACAC03;
	int d_BOR_KUCHYSAPO01;
	int d_BOR_KUCHYSAPO02;
	int d_BOR_KUCHYSAPO03;
	int d_BOR_KUCHYIBUSE01;
	int d_BOR_KUCHYIBUSE02;
	int d_BOR_KUCHYIBUSE03;
	int d_BOR_KUCHYTAYUYA01;
	int d_BOR_KUCHYTAYUYA02;
	int d_BOR_KUCHYTAYUYA03;
	int d_BOR_KUCHYKUGUTSU01;
	int d_BOR_KUCHYKUGUTSU02;
	int d_BOR_KUCHYKUGUTSU03;
	int d_BOR_BYAKUGAN;
	int d_BOR_ROKUJUYON;
	int d_BOR_8TRIG;
	int d_BOR_HIJUTSU;
	int d_BOR_SHARINT01;
	int d_BOR_SHARINT02;
	int d_BOR_SHARINT03;
	int d_BOR_SHAMSASUKE;
	int d_BOR_SHAMITACHI;
	int d_BOR_SHAMOBITO;
	int d_BOR_TSUKUYOMI;
	int d_BOR_SUSANOSASUKE;
	int d_BOR_SHINCHU;
	int d_BOR_KOKUANGYO;
	int d_BOR_KANENOOTO;
	int d_BOR_GENWAKU;
	int d_BOR_SHIBARI;
	int d_BOR_FCHAKRA;
	int d_BOR_NEHAN;
	int d_BOR_NARAKUMI;
	int d_BOR_KATON01;
	int d_BOR_SUITON01;
	int d_BOR_SUITON02;
	int d_BOR_SUITON03;
	int d_BOR_SUITON04;
	int d_BOR_KATON02;
	int d_BOR_KATON03;
	int d_BOR_KATON04;
	int d_BOR_KATON05;
	int d_BOR_MEDICINA01;
	int d_BOR_TERREMOTO;
	int d_BOR_RELAMPAGO;
	int d_BOR_RAITON_F;
	int d_BOR_SUITON05;
	int d_BOR_DOTON01;
	int d_BOR_NARA_03;
	int d_BOR_NARA_04;
	int d_BOR_NARA_05;
	int d_BOR_NARA_06;
	int d_BOR_BYAKUGAN_CONTROL;
	int d_BOR_RAITON_F2;

	int roulette_gold_step;
	int roulette_silver_step;
	int roulette_bronze_step;
};

/* criteria for battle_config.idletime_critera */
enum e_battle_config_idletime {
	BCIDLE_WALK          = 0x001,
	BCIDLE_USESKILLTOID  = 0x002,
	BCIDLE_USESKILLTOPOS = 0x004,
	BCIDLE_USEITEM       = 0x008,
	BCIDLE_ATTACK        = 0x010,
	BCIDLE_CHAT          = 0x020,
	BCIDLE_SIT           = 0x040,
	BCIDLE_EMOTION       = 0x080,
	BCIDLE_DROPITEM      = 0x100,
	BCIDLE_ATCOMMAND     = 0x200,
	BCIDLE_SCRIPT        = 0x400,
};

// Damage delayed info
struct delay_damage {
	int src_id;
	int target_id;
	int64 damage;
	int delay;
	unsigned short distance;
	uint16 skill_lv;
	uint16 skill_id;
	enum damage_lv dmg_lv;
	unsigned short attack_type;
	bool additional_effects;
	enum bl_type src_type;
};

/**
 * Battle.c Interface
 **/
struct battle_interface {
	/* */
	struct Battle_Config *bc;
	/* */
	int attr_fix_table[4][ELE_MAX][ELE_MAX];
	struct eri *delay_damage_ers; //For battle delay damage structures.
	/* init */
	void (*init) (bool minimal);
	/* final */
	void (*final) (void);
	/* damage calculation */
	struct Damage (*calc_attack) (int attack_type, struct block_list *bl, struct block_list *target, uint16 skill_id, uint16 skill_lv, int count);
	/* generic final damage calculation */
	int64 (*calc_damage) (struct block_list *src, struct block_list *bl, struct Damage *d, int64 damage, uint16 skill_id, uint16 skill_lv);
	/* gvg final damage calculation */
	int64 (*calc_gvg_damage) (struct block_list *src, struct block_list *bl, int64 damage, int div_, uint16 skill_id, uint16 skill_lv, int flag);
	/* battlegrounds final damage calculation */
	int64 (*calc_bg_damage) (struct block_list *src, struct block_list *bl, int64 damage, int div_, uint16 skill_id, uint16 skill_lv, int flag);
	/* normal weapon attack */
	enum damage_lv (*weapon_attack) (struct block_list *bl, struct block_list *target, int64 tick, int flag);
	/* check is equipped ammo and this ammo allowed */
	bool (*check_arrows) (struct map_session_data *sd);
	/* calculate weapon attack */
	struct Damage (*calc_weapon_attack) (struct block_list *src,struct block_list *target,uint16 skill_id,uint16 skill_lv,int wflag);
	/* delays damage or skills by a timer */
	int (*delay_damage) (int64 tick, int amotion, struct block_list *src, struct block_list *target, int attack_type, uint16 skill_id, uint16 skill_lv, int64 damage, enum damage_lv dmg_lv, int ddelay, bool additional_effects);
	/* drain damage */
	void (*drain) (struct map_session_data *sd, struct block_list *tbl, int64 rdamage, int64 ldamage, int race, int boss);
	/* damage reflect */
	void (*reflect_damage) (struct block_list *target, struct block_list *src, struct Damage *wd,uint16 skill_id);
	/* trap reflect */
	void (*reflect_trap) (struct block_list *target, struct block_list *src, struct Damage *md, uint16 skill_id);
	/* attribute rate */
	int (*attr_ratio) (int atk_elem, int def_type, int def_lv);
	/* applies attribute modifiers */
	int64 (*attr_fix) (struct block_list *src, struct block_list *target, int64 damage, int atk_elem, int def_type, int def_lv);
	/* applies card modifiers */
	int64 (*calc_cardfix) (int attack_type, struct block_list *src, struct block_list *target, int nk, int s_ele, int s_ele_, int64 damage, int left, int flag);
	int64 (*calc_cardfix2) (struct block_list *src, struct block_list *bl, int64 damage, int s_ele, int nk, int flag);
	/* applies element modifiers */
	int64 (*calc_elefix) (struct block_list *src, struct block_list *target, uint16 skill_id, uint16 skill_lv, int64 damage, int nk, int n_ele, int s_ele, int s_ele_, bool left, int flag);
	/* applies mastery modifiers */
	int64 (*calc_masteryfix) (struct block_list *src, struct block_list *target, uint16 skill_id, uint16 skill_lv, int64 damage, int div, bool left, bool weapon);
	/* calculates chorus bonus */
	int (*calc_chorusbonus) (struct map_session_data *sd);
	/* applies skill modifiers */
	int (*calc_skillratio) (int attack_type, struct block_list *src, struct block_list *target, uint16 skill_id, uint16 skill_lv, int skillratio, int flag);
	/* applies size modifiers */
	int64 (*calc_sizefix) (struct map_session_data *sd, int64 damage, int type, int size,  bool ignore);
	/* get weapon damage */
	int64 (*calc_weapon_damage) (struct block_list *src, struct block_list *bl, uint16 skill_id, uint16 skill_lv, struct weapon_atk *watk, int nk, bool n_ele, short s_ele, short s_ele_, int size, int type, int flag, int flag2);
	/* applies defense reductions */
	int64 (*calc_defense) (int attack_type, struct block_list *src, struct block_list *target, uint16 skill_id, uint16 skill_lv, int64 damage, int flag, int pdef);
	/* get master (who does this unit respond to?) */
	struct block_list *(*get_master) (struct block_list *src);
	/* returns a random unit who is targeting this unit */
	struct block_list *(*get_targeted) (struct block_list *target);
	/* picks a random enemy unit who is in the area of sight */
	struct block_list *(*get_enemy) (struct block_list *target, int type, int range);
	/* the target id (if any) of this unit */
	int (*get_target) (struct block_list *bl);
	/* the current skill being processed/casted by this unit */
	int (*get_current_skill) (struct block_list *bl);
	/* is either this race or element enough to be considered undead? */
	bool (*check_undead) (int race,int element);
	/* check if src and target are part of flag (e.g. enemies or allies) */
	int (*check_target) (struct block_list *src, struct block_list *target,int flag);
	/* is src and bl within range? */
	bool (*check_range) (struct block_list *src,struct block_list *bl,int range);
	/* consume ammo for this skill and lv */
	void (*consume_ammo) (struct map_session_data* sd, int skill_id, int lv);
	int (*get_targeted_sub) (struct block_list *bl, va_list ap);
	int (*get_enemy_sub) (struct block_list *bl, va_list ap);
	int (*get_enemy_area_sub) (struct block_list *bl, va_list ap);
	int (*delay_damage_sub) (int tid, int64 tick, int id, intptr_t data);
	int (*blewcount_bonus) (struct map_session_data *sd, uint16 skill_id);
	/* skill range criteria */
	int (*range_type) (struct block_list *src, struct block_list *target, uint16 skill_id, uint16 skill_lv);
	int64 (*calc_base_damage) (struct block_list *src, struct block_list *bl, uint16 skill_id, uint16 skill_lv, int nk, bool n_ele, short s_ele, short s_ele_, int type, int flag, int flag2);
	int64 (*calc_base_damage2) (struct status_data *st, struct weapon_atk *wa, struct status_change *sc, unsigned short t_size, struct map_session_data *sd, int flag);
	struct Damage (*calc_misc_attack) (struct block_list *src,struct block_list *target,uint16 skill_id,uint16 skill_lv,int mflag);
	struct Damage (*calc_magic_attack) (struct block_list *src,struct block_list *target,uint16 skill_id,uint16 skill_lv,int mflag);
	int (*adjust_skill_damage) (int m, unsigned short skill_id);
	int64 (*add_mastery) (struct map_session_data *sd,struct block_list *target,int64 dmg,int type);
	int (*calc_drain) (int64 damage, int rate, int per);
	/* - battle_config                           */
	int (*config_read) (const char *cfgName);
	int (*skill_config_read) (const char* cfgName);
	void (*config_set_defaults) (void);
	int (*config_set_value) (const char *w1, const char *w2);
	int (*config_get_value) (const char* w1);
	void (*config_adjust) (void);
	/* ----------------------------------------- */
	/* picks a random enemy within the specified range */
	struct block_list* (*get_enemy_area) (struct block_list *src, int x, int y, int range, int type, int ignore_id);
	/* damages area, originally for royal guard's reflect damage */
	int (*damage_area) (struct block_list *bl, va_list ap);
	void (*calc_masteryfix_unknown) (struct block_list *src, struct block_list *target, uint16 *skill_id, uint16 *skill_lv, int64 *damage, int *div, bool *left, bool *weapon);
	void (*calc_skillratio_magic_unknown) (int *attack_type, struct block_list *src, struct block_list *target, uint16 *skill_id, uint16 *skill_lv, int *skillratio, int *flag);
	void (*calc_skillratio_weapon_unknown) (int *attack_type, struct block_list *src, struct block_list *target, uint16 *skill_id, uint16 *skill_lv, int *skillratio, int *flag);
	void (*calc_misc_attack_unknown) (struct block_list *src, struct block_list *target, uint16 *skill_id, uint16 *skill_lv, int *mflag, struct Damage *md);

	//sistema de balanceamento
	//pedrodks
	bool (*skill_damage_iscaster) (struct block_list* src, struct block_list* target, int src_type, int target_type);
	int (*skill_damage_skill) (struct block_list* src, struct block_list* target, uint16 skill_id, uint16 skill_lv);
	int (*skill_damage_map) (struct block_list* src, struct block_list* target, uint16 skill_id, uint16 skill_lv);
	int (*skill_damage) (struct block_list* src, struct block_list* target, uint16 skill_id, uint16 skill_lv);
};

struct battle_interface *battle;

#ifdef BRATHENA_CORE
extern struct Battle_Config battle_config;
void battle_defaults(void);
#endif // BRATHENA_CORE

#endif /* MAP_BATTLE_H */
