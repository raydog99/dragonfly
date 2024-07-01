use std::collections::HashSet;

type Env = String;
type KAddr = String;
type KStore = String;
type Time = u64;
type Store = String;
type Exp = String;

#[derive(Clone)]
struct Psi(Env, KAddr, KStore, Time);

type M<A> = Box<dyn Fn(Psi, Store) -> HashSet<(A, Psi, Store)>>;

type Sigma = HashSet<(Exp, Psi, Store)>;

fn return_m<A: Clone + 'static>(x: A) -> M<A> {
    Box::new(move |psi, s| {
        let mut set = HashSet::new();
        set.insert((x.clone(), psi, s));
        set
    })
}

fn bind<A: Clone + 'static, B: Clone + 'static>(
    x: M<A>,
    f: Box<dyn Fn(A) -> M<B>>,
) -> M<B> {
    Box::new(move |psi, s| {
        x(psi.clone(), s.clone())
            .into_iter()
            .flat_map(|(a, psi_prime, s_prime)| f(a)(psi_prime, s_prime))
            .collect()
    })
}

fn get_env() -> M<Env> {
    Box::new(|psi, s| {
        let mut set = HashSet::new();
        set.insert((psi.0.clone(), psi, s));
        set
    })
}

fn put_env(rho_prime: Env) -> M<()> {
    Box::new(move |mut psi, s| {
        let mut set = HashSet::new();
        psi.0 = rho_prime.clone();
        set.insert(((), psi, s));
        set
    })
}

fn mzero<A>() -> M<A> {
    Box::new(|_, _| HashSet::new())
}

fn mplus<A: Clone + 'static>(x1: M<A>, x2: M<A>) -> M<A> {
    Box::new(move |psi, s| {
        let mut set = x1(psi.clone(), s.clone());
        set.extend(x2(psi, s));
        set
    })
}

fn alpha(f: Box<dyn Fn(Sigma) -> Sigma>) -> Box<dyn Fn(Exp) -> M<Exp>> {
    Box::new(move |e| {
        Box::new(move |psi, s| {
            let mut set = HashSet::new();
            set.insert((e.clone(), psi.clone(), s.clone()));
            f(set)
        })
    })
}

fn gamma(f: Box<dyn Fn(Exp) -> M<Exp>>) -> Box<dyn Fn(Sigma) -> Sigma> {
    Box::new(move |sigma| {
        sigma
            .into_iter()
            .flat_map(|(e, psi, s)| f(e)(psi, s))
            .collect()
    })
}