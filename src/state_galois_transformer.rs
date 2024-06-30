use std::collections::HashMap;

trait AbstractDomain {
    fn join(&self, other: &Self) -> Self;
    fn widen(&self, other: &Self) -> Self;
    fn is_subset(&self, other: &Self) -> bool;
}

#[derive(Clone, PartialEq, Eq, Hash)]
struct ConcreteState {
    variables: HashMap<String, i32>,
}

#[derive(Clone, PartialEq)]
struct AbstractState {
    variables: HashMap<String, (i32, i32)>,
}

impl AbstractDomain for AbstractState {
    fn join(&self, other: &Self) -> Self {
        let mut result = self.clone();
        for (var, (min, max)) in &other.variables {
            if let Some((self_min, self_max)) = result.variables.get_mut(var) {
                *self_min = (*self_min).min(*min);
                *self_max = (*self_max).max(*max);
            } else {
                result.variables.insert(var.clone(), (*min, *max));
            }
        }
        result
    }

    fn widen(&self, other: &Self) -> Self {
        let mut result = self.clone();
        for (var, (min, max)) in &other.variables {
            if let Some((self_min, self_max)) = result.variables.get_mut(var) {
                if *min < *self_min {
                    *self_min = i32::MIN;
                }
                if *max > *self_max {
                    *self_max = i32::MAX;
                }
            } else {
                result.variables.insert(var.clone(), (*min, *max));
            }
        }
        result
    }

    fn is_subset(&self, other: &Self) -> bool {
        for (var, (self_min, self_max)) in &self.variables {
            if let Some((other_min, other_max)) = other.variables.get(var) {
                if self_min < other_min || self_max > other_max {
                    return false;
                }
            } else {
                return false;
            }
        }
        true
    }
}

struct GaloisConnection;

impl GaloisConnection {
    fn alpha(&self, concrete: &ConcreteState) -> AbstractState {
        let mut abstract_state = AbstractState { variables: HashMap::new() };
        for (var, value) in &concrete.variables {
            abstract_state.variables.insert(var.clone(), (*value, *value));
        }
        abstract_state
    }

    fn gamma(&self, abstract: &AbstractState) -> Vec<ConcreteState> {
        let mut concrete_state = ConcreteState { variables: HashMap::new() };
        for (var, (min, max)) in &abstract.variables {
            concrete_state.variables.insert(var.clone(), (*min + *max) / 2);
        }
        vec![concrete_state]
    }
}

struct StateGaloisTransformer {
    galois_connection: GaloisConnection,
}

impl StateGaloisTransformer {
    fn new() -> Self {
        Self {
            galois_connection: GaloisConnection,
        }
    }

    fn transfer_function(&self, abstract_state: &AbstractState) -> AbstractState {
        abstract_state.clone()
    }

    fn analyze(&self, initial_state: &ConcreteState) -> AbstractState {
        let mut abstract_state = self.galois_connection.alpha(initial_state);
        let mut prev_state = AbstractState { variables: HashMap::new() };

        while !abstract_state.is_subset(&prev_state) {
            prev_state = abstract_state.clone();
            abstract_state = self.transfer_function(&abstract_state);
            abstract_state = abstract_state.widen(&prev_state);
        }

        abstract_state
    }
}