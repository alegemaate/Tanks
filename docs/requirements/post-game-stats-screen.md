# Post-Game Stats Screen

**Status:** Draft
**Priority:** High
**Created:** 2026-02-04
**Last Updated:** 2026-02-04 (rev 2)

## Overview

Add win/lose conditions to the game and a post-game stats screen that displays after a round ends. The screen shows aggregated gameplay statistics for all tanks and provides options to return to the menu or replay with the same settings.

## Goals

- Define clear win and lose conditions to give rounds a definitive end
- Track gameplay statistics globally across all tanks during a round
- Present stats in a post-game screen consistent with the existing UI style
- Provide replay and menu navigation options from the post-game screen
- Extract reusable UI components for consistency across screens

## Non-Goals / Out of Scope

- Per-tank stat breakdowns (stats are global aggregates only)
- Persistent stats across multiple rounds or sessions
- Leaderboards or high-score tracking
- Mid-game stats overlay or HUD stats display
- Achievements or unlockables tied to stats

## User Stories

### US-001: Win Condition

**As a** player, **I want** the round to end in victory when all enemy tanks are destroyed, **so that** there is a clear objective and sense of accomplishment.

**Priority:** Critical

**Acceptance Criteria:**

- [ ] Round ends immediately when the last enemy tank (team 1) is destroyed
- [ ] A "Victory" header is displayed on the post-game screen
- [ ] A victory sound effect plays when the win condition is met

### US-002: Lose Condition

**As a** player, **I want** the round to end in defeat when all friendly tanks (player and allies) are destroyed, **so that** there is meaningful risk during gameplay.

**Priority:** Critical

**Acceptance Criteria:**

- [ ] Round ends immediately when the last friendly tank (team 0, including the player) is destroyed
- [ ] A "Defeat" header is displayed on the post-game screen
- [ ] A defeat sound effect plays when the lose condition is met

### US-003: Bullets Fired Stat

**As a** player, **I want** to see the total number of bullets fired during the round, **so that** I can understand how active the battle was.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] Every bullet created by any tank increments a global "Bullets Fired" counter
- [ ] The total is displayed on the post-game screen with the label "Bullets Fired"

### US-004: Damage Taken Stat

**As a** player, **I want** to see the total damage received by all tanks during the round, **so that** I can gauge how intense the round was.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] Every point of damage a tank receives increments a global "Damage Taken" counter (tracked from the target's perspective)
- [ ] The total is displayed on the post-game screen with the label "Damage Taken"

### US-005: Hits Stat

**As a** player, **I want** to see the total number of bullet hits on tanks, **so that** I can understand shot accuracy across the round.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] Every bullet-to-tank collision increments a global "Hits" counter
- [ ] The total is displayed on the post-game screen with the label "Hits"

### US-006: Damage Done Stat

**As a** player, **I want** to see the total damage inflicted by all tanks on others during the round, **so that** I can see overall destructive output.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] Every point of damage a bullet deals to a tank increments a global "Damage Done" counter (tracked from the shooter's perspective)
- [ ] The total is displayed on the post-game screen with the label "Damage Done"

### US-007: Healing Done Stat

**As a** player, **I want** to see the total healing done during the round, **so that** I can see how much health was recovered from powerups.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] Every point of health restored by a HEALTH powerup increments a global "Healing Done" counter
- [ ] Health capped at max (100) should only count the effective healing (e.g., tank at 90 HP picking up +25 counts as 10)
- [ ] The total is displayed on the post-game screen with the label "Healing Done"

### US-008: Powerups Picked Up Stat

**As a** player, **I want** to see the total number of powerups collected during the round, **so that** I can see how many resources were claimed.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] Every powerup pickup by any tank increments a global "Powerups Picked Up" counter
- [ ] All powerup types count equally (HEALTH, SPEED, FIRE_SPEED, FIRE_DELAY)
- [ ] The total is displayed on the post-game screen with the label "Powerups Picked Up"

### US-009: Post-Game Screen Navigation

**As a** player, **I want** to return to the menu or replay the round from the post-game screen, **so that** I can continue playing without restarting the application.

**Priority:** High

**Acceptance Criteria:**

- [ ] A "Play Again" button starts a new round with the same settings (enemy count, ally count, map size, bounce count)
- [ ] A "Menu" button returns to the main menu
- [ ] Both buttons follow the existing Button UI component style

### US-010: Round Duration Stat

**As a** player, **I want** to see how long the round lasted, **so that** I can gauge the pace of the battle.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] A timer tracks elapsed time from round start to the win/lose trigger
- [ ] The duration is displayed on the post-game screen with the label "Round Duration"
- [ ] The time is formatted as minutes and seconds (e.g., "2m 34s")

### US-011: Reusable UI Components

**As a** developer, **I want** shared UI elements extracted into reusable components, **so that** the menu, game, and post-game screens have a consistent look and feel.

**Priority:** Medium

**Acceptance Criteria:**

- [ ] The existing Button component remains in `src/ui/` and is used across all screens
- [ ] Any new shared UI elements (e.g., text labels, stat rows, screen layout helpers) are placed in `src/ui/`
- [ ] The post-game screen reuses these components rather than duplicating UI logic

## Non-Functional Requirements

### Performance

- Stats tracking should have negligible impact on gameplay frame rate
- Post-game screen should render at the same frame rate as the menu

### Security

- N/A (single-player local game)

### Scalability

- Stats counters should use integer types large enough to handle long rounds (hundreds of tanks, thousands of bullets)

### Accessibility

- Stat labels and values should be clearly readable at the 800x600 game resolution
- Sufficient contrast between text and background

## Technical Constraints

- Must use C++20 and the existing ASW framework (v0.5.11)
- New screen must integrate with the existing `asw::scene::SceneManager<States>` pattern
- A new state (e.g., `States::PostGame`) will need to be added to the state enum
- Stats must be collected during the Game state and passed or made accessible to the PostGame state
- Must compile for both native (SDL2) and Emscripten (WebAssembly) targets

## Dependencies

- Existing `Button` component in `src/ui/`
- ASW scene management (`asw::scene::Scene`, `asw::scene::SceneManager`)
- Win/lose conditions (US-001, US-002) must be implemented before the post-game screen can be triggered

## Open Questions

- [x] ~~Should "Damage Taken" and "Damage Done" always be equal?~~ Resolved: Both are shown. "Damage Taken" is from the target's perspective, "Damage Done" is from the shooter's perspective. With global aggregation and bullets as the only damage source, values will be equal, but both are displayed for clarity.
- [x] ~~Should the stats screen show round duration?~~ Resolved: Yes, added as US-010.

## Changelog

| Date | Change | Author |
|------|--------|--------|
| 2026-02-04 | Initial draft | Product Owner |
| 2026-02-04 | Clarified Damage Taken vs Damage Done perspectives; added US-010 Round Duration stat; resolved open questions | Product Owner |
