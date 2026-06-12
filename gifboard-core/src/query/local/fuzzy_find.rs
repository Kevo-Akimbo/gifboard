#[derive(Copy, Clone, Default)]
pub(crate) struct Cell {
    score: i32,
    streak: i32,
}

pub(crate) fn smith_waterman<T: PartialEq, S, B>(
    substitution: S,
    boundary: B,
    haystack: &[T],
    needle: &[T],
    scores: &mut Vec<Cell>,
    vgaps: &mut Vec<i32>,
    hgaps: &mut Vec<i32>,
) -> i32
where
    S: Fn(&T, &T) -> i32,
    B: Fn(Option<&T>, &T) -> bool,
{
    let mut max_score = 0;
    let gap_extension_penalty = 2;
    let gap_start_penalty = 3;
    let streak_bonus = 1;

    let row_width = needle.len() + 1;
    scores.resize((haystack.len() + 1) * (needle.len() + 1), Cell::default());
    vgaps.resize((haystack.len() + 1) * (needle.len() + 1), 0);
    hgaps.resize((haystack.len() + 1) * (needle.len() + 1), 0);

    scores.fill(Cell::default());
    vgaps.fill(0);
    hgaps.fill(0);

    for i in 1..=haystack.len() {
        for j in 1..=needle.len() {
            let cur_idx = i * row_width + j;
            let diag_idx = (i - 1) * row_width + (j - 1);
            let up_idx = (i - 1) * row_width + j;
            let left_idx = i * row_width + (j - 1);

            let is_match = substitution(&haystack[i - 1], &needle[j - 1]) > 0;
            let diag = scores[diag_idx].score
                + substitution(&haystack[i - 1], &needle[j - 1])
                + scores[diag_idx].streak.min(4) * streak_bonus
                + if is_match && i >= 2 && boundary(haystack.get(i - 2), &haystack[i - 1]) {
                    1
                } else {
                    0
                };
            let vgap = (scores[up_idx].score - (gap_start_penalty + gap_extension_penalty))
                .max(vgaps[up_idx] - gap_extension_penalty);
            vgaps[cur_idx] = vgap;
            let hgap = (scores[left_idx].score - (gap_start_penalty + gap_extension_penalty))
                .max(hgaps[left_idx] - gap_extension_penalty);
            hgaps[cur_idx] = hgap;

            let mut score = 0;
            let mut streak = 0;
            if diag > score {
                score = diag;
                streak = scores[diag_idx].streak + 1;
            }
            if vgap > score {
                score = vgap;
                streak = 0;
            }
            if hgap > score {
                score = hgap;
                streak = 0;
            }
            if score > max_score {
                max_score = score;
            }
            scores[cur_idx] = Cell { score, streak }
        }
    }

    max_score
}
