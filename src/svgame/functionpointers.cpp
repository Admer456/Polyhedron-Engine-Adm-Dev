//
// This file is rather annoying/complicated, we'll have to seek for some way to replace it.
// 
// For now though, it works so... keep it simple stupid, don't change it if it ain't broken :)
//
#include "g_local.h"
#include "effects.h"
#include "utils.h"

// Entity includes.
#include "entities/func/func_clock.h"

// 
// ---- generated by grep & sed, do not edit = WRONG!!! Haha
// This wasn't generated anymore at all in our code, ah well :P
//
#include "functionpointers.h"

extern mmove_t soldier_move_attack1;
extern mmove_t soldier_move_attack2;
extern mmove_t soldier_move_attack3;
extern mmove_t soldier_move_attack4;
extern mmove_t soldier_move_attack6;
extern mmove_t soldier_move_death1;
extern mmove_t soldier_move_death2;
extern mmove_t soldier_move_death3;
extern mmove_t soldier_move_death4;
extern mmove_t soldier_move_death5;
extern mmove_t soldier_move_death6;
extern mmove_t soldier_move_duck;
extern mmove_t soldier_move_pain1;
extern mmove_t soldier_move_pain2;
extern mmove_t soldier_move_pain3;
extern mmove_t soldier_move_pain4;
extern mmove_t soldier_move_run;
extern mmove_t soldier_move_stand1;
extern mmove_t soldier_move_stand3;
extern mmove_t soldier_move_start_run;
extern mmove_t soldier_move_walk1;
extern mmove_t soldier_move_walk2;

extern void door_blocked(edict_t* self, edict_t* other);
//extern void door_secret_blocked(void);
extern void plat_blocked(edict_t* self, edict_t* other);
extern void rotating_blocked(edict_t* self, edict_t* other);
extern void train_blocked(edict_t* self, edict_t* other);
extern void turret_blocked(edict_t* self, edict_t* other);
//extern void actor_die(void);
extern void barrel_delay(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
//extern void berserk_die(void);
extern void body_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
//extern void boss2_die(void);
//extern void brain_die(void);
extern void button_killed(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
//extern void chick_die(void);
extern void debris_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
extern void door_killed(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
//extern void door_secret_die(void);
extern void flipper_die(void);
extern void floater_die(void);
extern void flyer_die(void);
extern void func_explosive_explode(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
extern void gib_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
extern void misc_deadsoldier_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
// extern void player_die(void); v 
extern void soldier_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
extern void turret_driver_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, const vec3_t& point);
//extern void actor_attack(void);
//extern void boss2_attack(void);
//extern void chick_attack(void);
//extern void floater_attack(void);
//extern void flyer_attack(void);
//extern void gladiator_attack(void);
//extern void gunner_attack(void);
//extern void hover_start_attack(void);
//extern void infantry_attack(void);
//extern void jorg_attack(void);
//extern void makron_attack(void);
//extern void medic_attack(void);
//extern void mutant_jump(void);
//extern void parasite_attack(void);
extern void soldier_attack(edict_t *self);
//extern void supertank_attack(void);
//extern void tank_attack(void);
extern void Boss2_CheckAttack(void);
extern void Jorg_CheckAttack(void);
extern void Makron_CheckAttack(void);
// extern void M_CheckAttack(void); // CPP: Remove cuz of overloaded func error.
extern void medic_checkattack(void);
extern void mutant_checkattack(void);
extern void soldier_dodge(edict_t* self, edict_t* attacker, float eta);
//extern void brain_idle(void);
//extern void floater_idle(void);
//extern void flyer_idle(void);
//extern void gladiator_idle(void);
//extern void infantry_fidget(void);
//extern void medic_idle(void);
//extern void mutant_idle(void);
//extern void parasite_idle(void);
//extern void tank_idle(void);
//extern void berserk_melee(void);
//extern void brain_melee(void);
//extern void chick_melee(void);
//extern void flipper_melee(void);
//extern void floater_melee(void);
//extern void flyer_melee(void);
//extern void gladiator_melee(void);
//extern void mutant_melee(void);
//extern void actor_run(void);
//extern void berserk_run(void);
//extern void boss2_run(void);
//extern void brain_run(void);
//extern void chick_run(void);
//extern void flipper_start_run(void);
//extern void floater_run(void);
//extern void flyer_run(void);
//extern void gladiator_run(void);
//extern void gunner_run(void);
//extern void hover_run(void);
//extern void infantry_run(void);
//extern void insane_run(void);
//extern void jorg_run(void);
//extern void makron_run(void);
//extern void medic_run(void);
//extern void mutant_run(void);
//extern void parasite_start_run(void);
extern void soldier_run(edict_t *self);
//extern void supertank_run(void);
//extern void tank_run(void);
//extern void berserk_search(void);
//extern void boss2_search(void);
//extern void brain_search(void);
//extern void gladiator_search(void);
//extern void gunner_search(void);
//extern void hover_search(void);
//extern void jorg_search(void);
//extern void medic_search(void);
//extern void mutant_search(void);
//extern void supertank_search(void);
//extern void berserk_sight(void);
//extern void brain_sight(void);
//extern void chick_sight(void);
//extern void flipper_sight(void);
//extern void floater_sight(void);
//extern void flyer_sight(void);
//extern void gladiator_sight(void);
//extern void gunner_sight(void);
//extern void hover_sight(void);
//extern void infantry_sight(void);
//extern void makron_sight(void);
//extern void medic_sight(void);
//extern void mutant_sight(void);
//extern void parasite_sight(void);
extern void soldier_sight(edict_t* self, edict_t* other);
//extern void tank_sight(void);
//extern void actor_stand(void);
//extern void berserk_stand(void);
//extern void boss2_stand(void);
//extern void brain_stand(void);
//extern void chick_stand(void);
//extern void flipper_stand(void);
//extern void floater_stand(void);
//extern void flyer_stand(void);
//extern void gladiator_stand(void);
//extern void gunner_stand(void);
//extern void hover_stand(void);
//extern void infantry_stand(void);
//extern void insane_stand(void);
//extern void jorg_stand(void);
//extern void makron_stand(void);
//extern void medic_stand(void);
//extern void mutant_stand(void);
//extern void parasite_stand(void);
extern void soldier_stand(edict_t *self);
//extern void supertank_stand(void);
//extern void tank_stand(void);
//extern void actor_walk(void);
//extern void berserk_walk(void);
//extern void boss2_walk(void);
//extern void brain_walk(void);
//extern void chick_walk(void);
//extern void flipper_walk(void);
//extern void floater_walk(void);
//extern void flyer_walk(void);
//extern void gladiator_walk(void);
//extern void gunner_walk(void);
//extern void hover_walk(void);
//extern void infantry_walk(void);
//extern void insane_walk(void);
//extern void jorg_walk(void);
//extern void makron_walk(void);
//extern void medic_walk(void);
//extern void mutant_walk(void);
//extern void parasite_start_walk(void);
extern void soldier_walk(edict_t *self);
//extern void supertank_walk(void);

//extern void player_pain(void); // CPP: Remove cuz of overloaded func error.
extern void soldier_pain(edict_t* self, edict_t* other, float kick, int damage);
//extern void supertank_pain(void);
//extern void tank_pain(void);
extern void misc_viper_bomb_prethink(edict_t *self);
extern void Brush_AngleMove_Begin(edict_t* self);
extern void Brush_AngleMove_Done(edict_t* self);
extern void Brush_AngleMove_Final(edict_t* self);
extern void barrel_explode(edict_t* self);
extern void bfg_explode(edict_t* self);
extern void bfg_think(edict_t* self);

extern void button_return(edict_t* self);

extern void door_go_down(edict_t* self);

extern void DoRespawn(edict_t* self);
extern void drop_make_touchable(edict_t* self);
extern void droptofloor(edict_t* self);
extern void flymonster_start_go(edict_t* self);
//extern void func_clock_think(void);// CPP: Remove cuz of overloaded func error.
extern void func_object_release(edict_t* self);
extern void func_timer_think(edict_t* self);
extern void func_train_find(edict_t* self);
//extern void G_FreeEdict(void); // CPP: Remove cuz of overloaded func error.
extern void gib_think(edict_t* self);
extern void Grenade_Explode(edict_t* self);

// extern void M_droptofloor(void); // CPP: Remove cuz of overloaded func error.
extern void MegaHealth_think(edict_t* self);
extern void M_FliesOff(edict_t* self);
extern void M_FliesOn(edict_t* self);
extern void misc_banner_think(edict_t* self);
extern void misc_blackhole_think(edict_t* self);
extern void misc_easterchick2_think(edict_t* self);
extern void misc_easterchick_think(edict_t* self);
extern void misc_eastertank_think(edict_t* self);
extern void misc_satellite_dish_think(edict_t* self);
//extern void monster_think(void); // CPP: Remove cuz of overloaded func error.
extern void monster_triggered_spawn(edict_t* self);
extern void Brush_Move_Begin(edict_t* self);
extern void Brush_Move_Done(edict_t* self);
extern void Brush_Move_Final(edict_t* self);
extern void multi_wait(edict_t* self);
extern void plat_go_down(edict_t* self);
extern void SP_CreateCoopSpots(edict_t* self);
extern void SP_FixCoopSpots(edict_t* self);
extern void swimmonster_start_go(edict_t* self);
extern void target_crosslevel_target_think(edict_t* self);
extern void target_earthquake_think(edict_t* self);
extern void target_explosion_explode(edict_t* self);
extern void target_laser_start(edict_t* self);
extern void target_laser_think(edict_t* self);
extern void target_lightramp_think(edict_t* self);
extern void Think_AccelMove(edict_t* self);

extern void Think_CalcMoveSpeed(edict_t* self);
extern void Think_Delay(edict_t* self);
extern void Think_SpawnDoorTrigger(edict_t* self);
extern void TH_viewthing(edict_t* self);
extern void train_next(edict_t* self);
extern void trigger_elevator_init(edict_t* self);
extern void turret_breach_finish_init(edict_t* self);
extern void turret_breach_think(edict_t* self);
extern void turret_driver_link(edict_t* self);
extern void turret_driver_think(edict_t* self);
extern void walkmonster_start_go(edict_t* self);
extern void barrel_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void bfg_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void blaster_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void button_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void door_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void drop_temp_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void func_object_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void gib_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void Grenade_Touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void hurt_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void misc_viper_bomb_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);

extern void path_corner_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void point_combat_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void rocket_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void rotating_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);

extern void teleporter_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void Touch_DoorTrigger(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
//extern void Touch_Item(void); // CPP: Remove cuz of overloaded func error.
extern void Touch_Multi(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void Touch_Plat_Center(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void trigger_gravity_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void trigger_monsterjump_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);
extern void trigger_push_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf);

extern void button_use(edict_t* self, edict_t* other, edict_t* activator);
//extern void commander_body_use(void);
//extern void door_secret_use(void);
extern void door_use(edict_t* self, edict_t* other, edict_t* activator);
//extern void func_clock_use(void); // CPP: Remove cuz of overloaded func error.
//extern void func_conveyor_use(edict_t* self, edict_t* other, edict_t* activator);
extern void func_conveyor_use(edict_t* self, edict_t* other, edict_t* activator);
extern void func_explosive_spawn(edict_t* self, edict_t* other, edict_t* activator);
extern void func_explosive_use(edict_t* self, edict_t* other, edict_t* activator);
extern void func_object_use(edict_t* self, edict_t* other, edict_t* activator);
extern void func_timer_use(edict_t* self, edict_t* other, edict_t* activator);
extern void func_wall_use(edict_t* self, edict_t* other, edict_t* activator);
extern void hurt_use(edict_t* self, edict_t* other, edict_t* activator);
extern void light_use(edict_t* self, edict_t* other, edict_t* activator);
extern void misc_blackhole_use(edict_t* self, edict_t* other, edict_t* activator);
extern void misc_satellite_dish_use(edict_t* self, edict_t* other, edict_t* activator);
extern void misc_strogg_ship_use(edict_t* self, edict_t* other, edict_t* activator);
extern void misc_viper_bomb_use(edict_t* self, edict_t* other, edict_t* activator);
extern void misc_viper_use(edict_t* self, edict_t* other, edict_t* activator);
extern void monster_triggered_spawn_use(edict_t* self, edict_t* other, edict_t* activator);
extern void monster_use(edict_t* self, edict_t* other, edict_t* activator);
extern void rotating_use(edict_t* self, edict_t* other, edict_t* activator);
extern void target_earthquake_use(edict_t* self, edict_t* other, edict_t* activator);
extern void target_laser_use(edict_t* self, edict_t* other, edict_t* activator);
extern void target_lightramp_use(edict_t* self, edict_t* other, edict_t* activator);
extern void target_string_use(edict_t* self, edict_t* other, edict_t* activator);
extern void train_use(edict_t* self, edict_t* other, edict_t* activator);
extern void trigger_counter_use(edict_t* self, edict_t* other, edict_t* activator);
extern void trigger_crosslevel_trigger_use(edict_t* self, edict_t* other, edict_t* activator);
extern void trigger_elevator_use(edict_t* self, edict_t* other, edict_t* activator);
extern void trigger_enable(edict_t* self, edict_t* other, edict_t* activator);
extern void trigger_key_use(edict_t* self, edict_t* other, edict_t* activator);
extern void trigger_relay_use(edict_t* self, edict_t* other, edict_t* activator);
extern void Use_Areaportal(edict_t* self, edict_t* other, edict_t* activator);

extern void Use_Item(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_killbox(edict_t* ent, edict_t* other, edict_t* activator);
extern void Use_Multi(edict_t* ent, edict_t* other, edict_t* activator);
extern void Use_Plat(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_target_blaster(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_target_changelevel(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_target_explosion(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_target_goal(edict_t* ent, edict_t* other, edict_t* activator);
extern void Use_Target_Help(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_target_secret(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_target_spawner(edict_t* ent, edict_t* other, edict_t* activator);
extern void Use_Target_Speaker(edict_t* ent, edict_t* other, edict_t* activator);
extern void use_target_splash(edict_t* ent, edict_t* other, edict_t* activator);
extern void Use_Target_Tent(edict_t* ent, edict_t* other, edict_t* activator);
extern void plat_hit_bottom(edict_t* self);
extern void plat_hit_top(edict_t* self);
extern void button_done(edict_t* self);
extern void button_wait(edict_t* self);
extern void door_hit_bottom(edict_t* self);
extern void door_hit_top(edict_t* self);
extern void train_wait(edict_t* self);

extern void flare_think(edict_t* self); // Q2RTX
extern void flare_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf); // Q2RTX

const save_ptr_t save_ptrs[] = {
{ P_blocked, door_blocked },
{ P_blocked, plat_blocked },
{ P_blocked, rotating_blocked },
{ P_blocked, train_blocked },
{ P_blocked, turret_blocked },

{ P_die, barrel_delay },
{ P_die, body_die },
{ P_die, button_killed },
{ P_die, debris_die },
{ P_die, door_killed },

{ P_die, func_explosive_explode },

{ P_die, gib_die },
{ P_die, misc_deadsoldier_die },
{ P_die, player_die },
{ P_die, soldier_die },
{ P_die, turret_driver_die },

{ P_monsterinfo_attack, soldier_attack },

{ P_monsterinfo_checkattack, M_CheckAttack },

{ P_monsterinfo_currentmove, &soldier_move_attack1 },
{ P_monsterinfo_currentmove, &soldier_move_attack2 },
{ P_monsterinfo_currentmove, &soldier_move_attack3 },
{ P_monsterinfo_currentmove, &soldier_move_attack4 },
{ P_monsterinfo_currentmove, &soldier_move_attack6 },
{ P_monsterinfo_currentmove, &soldier_move_death1 },
{ P_monsterinfo_currentmove, &soldier_move_death2 },
{ P_monsterinfo_currentmove, &soldier_move_death3 },
{ P_monsterinfo_currentmove, &soldier_move_death4 },
{ P_monsterinfo_currentmove, &soldier_move_death5 },
{ P_monsterinfo_currentmove, &soldier_move_death6 },
{ P_monsterinfo_currentmove, &soldier_move_duck },
{ P_monsterinfo_currentmove, &soldier_move_pain1 },
{ P_monsterinfo_currentmove, &soldier_move_pain2 },
{ P_monsterinfo_currentmove, &soldier_move_pain3 },
{ P_monsterinfo_currentmove, &soldier_move_pain4 },
{ P_monsterinfo_currentmove, &soldier_move_run },
{ P_monsterinfo_currentmove, &soldier_move_stand1 },
{ P_monsterinfo_currentmove, &soldier_move_stand3 },
{ P_monsterinfo_currentmove, &soldier_move_start_run },
{ P_monsterinfo_currentmove, &soldier_move_walk1 },
{ P_monsterinfo_currentmove, &soldier_move_walk2 },


{ P_monsterinfo_dodge, soldier_dodge },

{ P_monsterinfo_run, soldier_run }, 

{ P_monsterinfo_sight, soldier_sight },

{ P_monsterinfo_stand, soldier_stand },

{ P_monsterinfo_walk, soldier_walk },

{ P_pain, player_pain },
{ P_pain, soldier_pain },

{ P_prethink, misc_viper_bomb_prethink },
{ P_think, Brush_AngleMove_Begin },
{ P_think, Brush_AngleMove_Done },
{ P_think, Brush_AngleMove_Final },
{ P_think, barrel_explode },
{ P_think, bfg_explode },
{ P_think, bfg_think },

{ P_think, button_return },

{ P_think, door_go_down },

{ P_think, DoRespawn },
{ P_think, drop_make_touchable },
{ P_think, droptofloor },
{ P_think, flymonster_start_go },
{ P_think, func_clock_think },
{ P_think, func_object_release },
{ P_think, func_timer_think },
{ P_think, func_train_find },
{ P_think, G_FreeEdict },
{ P_think, gib_think },
{ P_think, Grenade_Explode },

{ P_think, M_droptofloor },
{ P_think, MegaHealth_think },
{ P_think, M_FliesOff },
{ P_think, M_FliesOn },
{ P_think, misc_banner_think },
{ P_think, misc_blackhole_think },
{ P_think, misc_easterchick2_think },
{ P_think, misc_easterchick_think },
{ P_think, misc_eastertank_think },
{ P_think, misc_satellite_dish_think },
{ P_think, monster_think },
{ P_think, monster_triggered_spawn },
{ P_think, Brush_Move_Begin },
{ P_think, Brush_Move_Done },
{ P_think, Brush_Move_Final },
{ P_think, multi_wait },
{ P_think, plat_go_down },
{ P_think, SP_CreateCoopSpots },
{ P_think, SP_FixCoopSpots },
{ P_think, swimmonster_start_go },
{ P_think, target_crosslevel_target_think },
{ P_think, target_earthquake_think },
{ P_think, target_explosion_explode },
{ P_think, target_laser_start },
{ P_think, target_laser_think },
{ P_think, target_lightramp_think },
{ P_think, Think_AccelMove },

{ P_think, Think_CalcMoveSpeed },
{ P_think, Think_Delay },
{ P_think, Think_SpawnDoorTrigger },
{ P_think, TH_viewthing },
{ P_think, train_next },
{ P_think, trigger_elevator_init },
{ P_think, turret_breach_finish_init },
{ P_think, turret_breach_think },
{ P_think, turret_driver_link },
{ P_think, turret_driver_think },
{ P_think, walkmonster_start_go },
{ P_think, flare_think }, // Q2RTX
{ P_touch, flare_touch }, // Q2RTX
{ P_touch, barrel_touch },
{ P_touch, bfg_touch },
{ P_touch, blaster_touch },
{ P_touch, button_touch },
{ P_touch, door_touch },
{ P_touch, drop_temp_touch },
{ P_touch, func_object_touch },
{ P_touch, gib_touch },
{ P_touch, Grenade_Touch },
{ P_touch, hurt_touch },
{ P_touch, misc_viper_bomb_touch },

{ P_touch, path_corner_touch },
{ P_touch, point_combat_touch },
{ P_touch, rocket_touch },
{ P_touch, rotating_touch },


{ P_touch, teleporter_touch },

{ P_touch, Touch_DoorTrigger },
{ P_touch, Touch_Item },
{ P_touch, Touch_Multi },
{ P_touch, Touch_Plat_Center },

{ P_touch, trigger_gravity_touch },
{ P_touch, trigger_monsterjump_touch },
{ P_touch, trigger_push_touch },

{ P_use, button_use },

{ P_use, door_use },

{ P_use, func_clock_use },
{ P_use, func_conveyor_use },
{ P_use, func_explosive_spawn },
{ P_use, func_explosive_use },
{ P_use, func_object_use },
{ P_use, func_timer_use },
{ P_use, func_wall_use },

{ P_use, hurt_use },
{ P_use, light_use },

{ P_use, misc_blackhole_use },
{ P_use, misc_satellite_dish_use },
{ P_use, misc_strogg_ship_use },
{ P_use, misc_viper_bomb_use },
{ P_use, misc_viper_use },

{ P_use, monster_triggered_spawn_use },
{ P_use, monster_use },

{ P_use, rotating_use },

{ P_use, target_earthquake_use },
{ P_use, target_laser_use },
{ P_use, target_lightramp_use },
{ P_use, target_string_use },
{ P_use, train_use },

{ P_use, trigger_counter_use },
{ P_use, trigger_crosslevel_trigger_use },
{ P_use, trigger_elevator_use },
{ P_use, trigger_enable },
{ P_use, trigger_key_use },
{ P_use, trigger_relay_use },

{ P_use, Use_Areaportal },
{ P_use, Use_Item },
{ P_use, use_killbox },
{ P_use, Use_Multi },
{ P_use, Use_Plat },
{ P_use, use_target_blaster },
{ P_use, use_target_changelevel },
{ P_use, use_target_explosion },
{ P_use, use_target_goal },
{ P_use, Use_Target_Help },
{ P_use, use_target_secret },
{ P_use, use_target_spawner },
{ P_use, Use_Target_Speaker },
{ P_use, use_target_splash },
{ P_use, Use_Target_Tent },

{ P_moveinfo_endfunc, plat_hit_bottom },
{ P_moveinfo_endfunc, plat_hit_top },

{ P_moveinfo_endfunc, button_done },
{ P_moveinfo_endfunc, button_wait },

{ P_moveinfo_endfunc, door_hit_bottom },
{ P_moveinfo_endfunc, door_hit_top },

{ P_moveinfo_endfunc, train_wait },

};
const int num_save_ptrs = sizeof(save_ptrs) / sizeof(save_ptrs[0]);
