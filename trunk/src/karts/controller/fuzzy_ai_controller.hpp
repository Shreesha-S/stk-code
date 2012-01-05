// $Id: fuzzy_ai_controller.hpp 10039 2011-10-25 17:46:14Z auria $ 
// ID TODO KINSU
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2004-2005 Steve Baker <sjbaker1@airmail.net>
//  Copyright (C) 2006-2007 Eduardo Hernandez Munoz
//  Copyright (C) 2010      Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_FUZZY_AI_CONTROLLER_HPP
#define HEADER_FUZZY_AI_CONTROLLER_HPP

#include "karts/controller/ai_base_controller.hpp"

struct PathData;
class  Track;
class  LinearWorld;
class  QuadGraph;
class  FuzzyAITaggable;

namespace irr
{
    namespace scene
    {
        class ISceneNode;
        class IBillboardTextSceneNode;
    }
}


/**-----------------------------------------------------------------------------
 *
 */
struct AttrPoint
{
    float            x;
    float            z;          // Irrlicht-like Z-axis coordinate
    float            interest;
    float            difficulty; // difficulty to reach the point
    float            attraction;
    bool             updated;
    FuzzyAITaggable* object;     // Null if this is the track node attraction pt
    
    AttrPoint(float x = 0.0f, float z = 0.0f) : x(x), z(z),
                                                interest(0.f),
                                                difficulty(0.f),
                                                attraction(0.f),
                                                updated(false),
                                                object(NULL)    {}
};

/**
  * \ingroup controller
  */
class FuzzyAIController : public AIBaseController
{
private:
    /** How the AI uses nitro. */
    enum {NITRO_NONE, NITRO_SOME, NITRO_ALL} m_nitro_level;
    enum ItemTactic
    {
        IT_TEN_SECONDS, //Fire after 10 seconds have passed, since the item
                        //was grabbed.
        IT_CALCULATE //Aim carefully, check for enough space for boosters,
                     //and that other conditions are meet before firing.
    };

    class CrashTypes
    {
        public:

        bool m_road; //true if we are going to 'crash' with the bounds of the road
        int m_kart; //-1 if no crash, pos numbers are the kart it crashes with
        CrashTypes() : m_road(false), m_kart(-1) {};
        void clear() {m_road = false; m_kart = -1;}
    } m_crashes;
    
    //==========================================================================
    // Fuzzy AI variables
    static unsigned int     instanceCount; // used to determine which instance has to debug (the first one only)
    float                   m_timer;
    int                     m_compet; // Competitiveness of the agent TODO Constant values for this variable (LOW, HIGH, ...)
    int                     m_aggress; // Aggressiveness
    /* Collision tagger */
    // Vector<int> m_speed_diff;
    std::vector<AttrPoint*> m_attrPts;  // Attraction points
    AttrPoint               m_mainAPt;  // Main Attraction Point (straight point)
    int                     m_target_x; // Target point coords got by handleSteering()
    int                     m_target_z;
    AttrPoint *             m_chosenDir;
    bool                    debug;      // if this agent must print debug output
    unsigned int            instanceID; // for random
    // End of Fuzzy AI variables
    //==========================================================================
    
    /*Difficulty handling variables*/
    /** Chance of a false start. */
    float m_false_start_probability;
    /** The minimum delay time before a AI kart starts. */
    float m_min_start_delay;
    /** The maximum delay time before an AI kart starts. */
    float m_max_start_delay;
    /** The actual start delay used. */
    float m_start_delay; 
  
    /** Minimum number of steps to check. If 0, the AI doesn't even has check
     *  around the kart, if 1, it checks around the kart always, and more 
     *  than that will check the remaining number of steps in front of the 
     *  kart, always. */
    int m_min_steps; 
     /** If true, the acceleration is decreased when the AI is in a better 
      *  position than all the human players. */
    bool  m_wait_for_players;

    /** The allowed maximum speed in percent of the kart's maximum speed. */
    float m_max_handicap_speed; 
    
     /** How are items going to be used? */
    ItemTactic m_item_tactic;

    /** True if the kart should try to pass on a bomb to another kart. */
    bool m_handle_bomb;

    /** True if the AI should avtively try to make use of slipstream. */
    bool m_make_use_of_slipstream;

    /*General purpose variables*/
    //The crash percentage is how much of the time the AI has been crashing,
    //if the AI has been crashing for some time, use the rescue.
    float m_crash_time;
    int   m_collided;           // true if the kart collided with the track

    /** Pointer to the closest kart ahead of this kart. NULL if this
     *  kart is first. */
    Kart *m_kart_ahead;
    /** Distance to the kart ahead. */
    float m_distance_ahead;

    /** Pointer to the closest kart behind this kart. NULL if this kart
     *  is last. */
    Kart *m_kart_behind;
    /** Distance to the kard behind. */
    float m_distance_behind;

    /** Time an item has been collected and not used. */
    float m_time_since_last_shot;
  
    float m_curve_target_speed;
    float m_curve_angle;

    float m_time_since_stuck;

    int m_start_kart_crash_direction; //-1 = left, 1 = right, 0 = no crash.

    /** For debugging purpose: a sphere indicating where the AI 
     *  is targeting at. */
    irr::scene::ISceneNode *m_debug_sphere;

    /*Functions called directly from update(). They all represent an action
     *that can be done, and end up setting their respective m_controls
     *variable, except handle_race_start() that isn't associated with any
     *specific action (more like, associated with inaction).
     */
    void  handleRaceStart();
    void  handleAcceleration(const float dt);
    void  handleSteering(float dt);
    void  handleItems(const float dt);
    void  handleRescue(const float dt);
    void  handleBraking();
    void  handleNitroAndZipper();
    void  computeNearestKarts();

    void  checkCrashes(int steps, const Vec3& pos);
    void  findNonCrashingPoint(Vec3 *result);

    //--------------------------------------------------------------------------
    // Fuzzy AI methods

    int   computeCompetitiveness  (int          player_level,
                                   unsigned int current_ranking );

    int   computeAggressiveness   (unsigned int kart_class,
                                   unsigned int current_ranking );

    float computeDifficultyTag    (float        angle,
                                   int          direction,
                                   float        distance );

    float computeBoxAttraction    (float        difficultyTag,
                                   bool         hasPowerup);

    float computeCollisionAttraction(float difficultyTag,
                                     int   aggressiveness);
    
    float computeHitEstimation    (int          possessed_item_type,
                                   float        next_kart_distance );

    float computeWeaponInterest   (int          competitiveness,
                                   float        hit_estimation );
    
    float computeSpeedHandling    (float        difficulty,
                                   float        currentSpeed,
                                   int          competitiveness,
                                   int          skid);
    
    // -- Wrapper function for computeDifficultyTag --
    float estimateDifficultyToReach (const Vec3& point);
    
    // -- Detection methods --
    void  getCloseKarts           (std::vector<const Kart*>& closeKarts,
                                   float max_dist = 40.f);
    // -- Items tagging method --
    void  tagItems                (const std::vector<Item*>& items,
                                         std::vector<AttrPoint*>& output);
    // -- Collisions tagging method --
    void tagKartCollisions        (const std::vector<const Kart*>& karts,
                                         std::vector<AttrPoint*>&  output);
    // -- Path Choosing methods --
    // Fork detection in the look_ahead nodes
    void  computeForkChoices      (std::vector<unsigned int>& output);
    // Fuzzy path chooser for a given fork
    int   choosePath      (const std::vector<std::vector<PathData*>*>* pathData,
                                   float        competitiveness);
    
    AttrPoint* chooseDirection(std::vector<AttrPoint*> & attrPts);

    void handleAccelerationAndBrake(float dt);

    // -- Debug method --
//    void setDebugText(const Item* item, const std::string* text);
    //void  printFuzzyData();
    //--------------------------------------------------------------------------

    int   calcSteps();
    void  findCurve();
protected:
    virtual unsigned int getNextSector(unsigned int index);

public:
                 FuzzyAIController(Kart *kart);
                ~FuzzyAIController();
    virtual void update      (float delta) ;
    virtual void reset       ();
    virtual void crashed     (Kart *k) {if(k) m_collided = true;};
    virtual const irr::core::stringw& getNamePostfix() const;
};

#endif // #ifndef HEADER_FUZZY_AI_CONTROLLER_HPP

/* EOF */