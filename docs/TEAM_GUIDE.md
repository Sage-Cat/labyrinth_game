# Team Guide — Building **labyrinth_game** 

> Audience: 1 team lead + 5–10 juniors (few months of C++ basics).

This guide defines how we plan, split, implement, review, and ship the game together. It complements the development plan and repository README.

---

## 0) Goals (Why this is fun & doable)

- **Make a playable console roguelike** with a clean architecture and tests.
- **Juniors** learn: C++20 basics, modular design, Git workflow, code review, testing.
- **Lead** provides: project skeleton, interfaces, guardrails, weekly priorities.

---

## 1) Tools & Boards

- **Git hosting & issues:** GitHub (recommended)  
  - Use **GitHub Issues** + **GitHub Projects (board)** (free, integrated).

### Board Columns (Kanban)
`Backlog` → `Todo` → `In Progress` → `Review/Testing` → `Done`

### Labels
- **Difficulty:** `easy`, `medium`, `hard`  
- **Area:** `Domain`, `Application`, `Infrastructure`, `Documentation`, `Tests`
- **Type:** `bug`, `feature`
- (in future) **State:** `blocked`, `needs-lead-input`

---

## 2) Roles & Responsibilities

**Lead:**
- Owns **architecture**, **public interfaces**, **initial skeleton**, and hard refactors.
- Curates backlog, assigns difficulty, reviews PRs, sets weekly goals, merges.

**Juniors:**
- Pick **easy** tasks first; aim for **small, reviewable PRs**.
- Write tests when the task includes them; follow style and PR checklist.
- Ask early, push often, keep PRs under ~300 LOC where possible.

---

## 3) Working Agreements

- **Small issues, small PRs.** Keep scope focused.
- **One branch per issue.** Name: `feature/<issue-id>-short-name` or `bugfix/<issue-id>-short-name`.
- (in future) **Green CI before review.** Build + tests + sanitizers.

---

## 5) Contribution Workflow (step-by-step)

1. **Pick an issue** in `Todo` (or ask lead to move your choice from `Backlog` → `Todo`).
2. **Take issue into work** by moving it to `In Progress`
3. **Create a branch**: `feature/123-map-in-bounds-check`.
4. **Implement** (follow coding style, code formatting & tests). Commit early, push often.
5. **Open Pull Request** referencing the issue (`Fixes #123`) and fill the PR checklist.
6. **Request review** from lead (and peers).
7. **Address feedback** → re-request review.
8. **Can be merged** after approval.

---

## 6) Branching Model & Commit Style

- Branches: `main` (stable) / `dev` (integration) / `feature/*` / `bugfix/*`.
- Commits: imperative mood: `Add bounds check to Grid::at()`; one topic per commit.

---

## 7) Definition of Done (DoD)

- Feature works & has **tests** passing.
- **No new warnings**; sanitizers pass.
- **Docs updated** (README or module comment).
- **PR approved**; board card moved to `Done`.

---

## 8) Junior Onboarding Plan (first week)

**Day 1**
- Read `README.md` and this guide. Build & run “hello skeleton”.  
- Read layer overview in the development plan.

**Day 2**
- First **easy** task (pure function, no I/O).  
- Open PR; get feedback.

**Day 3–5**
- 2–3 more easy tasks.  
- Shadow a code review; write your first small test.

---

## 9) Weekly Cadence (lightweight)

- **Kickoff (Mon, 20–30 min):** lead sets priorities; assign tasks.
- **Async standups:** each student posts **yesterday/today/blocked** in the board card.
- **Review window (Daily, 30–60 min lead):** PR reviews + merges.
- **Demo (Fri, 30 min):** show the game progress; reflect and retune backlog.

---

## 10) Quick Start (juniors)

1. Clone, configure, and build (see README).  
2. Pick an **easy** issue; create a branch.
3. Implement, test, open PR, request review.
4. Iterate on feedback → learn → merge → next task.

---

## 20) License & Conduct

- Be respectful, give constructive feedback.  
- Ask questions early. We learn together.

---

*Happy hacking & may your paths be free of walls!* 🧭
