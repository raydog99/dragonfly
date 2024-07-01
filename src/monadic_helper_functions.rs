use std::collections::HashMap;
use std::time::SystemTime;

type Frame = Vec<String>; // Vector of strings (e.g., variable names)
type KAddr = usize; // Key address as an integer
type KStore = HashMap<KAddr, Vec<Frame>>; // Key store mapping addresses to frames
type Time = u64; // Time as a 64-bit unsigned integer
type Atom = String; // Atom as a string 
type Exp = String; // Expression as a string
type Env = HashMap<String, Atom>; // Environment mapping variable names to atoms
type Store = HashMap<Atom, bool>; // Store mapping atoms to boolean values

static mut CURRENT_KADDR: KAddr = 0;
static mut CURRENT_TIME: Time = 0;

fn get_kaddr() -> M<KAddr> {
    unsafe {
        CURRENT_KADDR += 1;
        M(CURRENT_KADDR)
    }
}

fn get_kstore() -> M<KStore> {
    M(HashMap::new())
}

fn get_time() -> M<Time> {
    unsafe {
        CURRENT_TIME += 1;
        M(CURRENT_TIME)
    }
}

fn put_kstore(ks: KStore) -> M<()> {
    println!("Updating KStore: {:?}", ks);
    M(())
}

fn put_kaddr(ka: KAddr) -> M<()> {
    unsafe {
        CURRENT_KADDR = ka;
        M(())
    }
}

fn get_env() -> M<Env> {
    let mut env = HashMap::new();
    env.insert("x".to_string(), "a".to_string());
    env.insert("y".to_string(), "b".to_string());
    M(env)
}

fn get_store() -> M<Store> {
    let mut store = HashMap::new();
    store.insert("a".to_string(), true);
    store.insert("b".to_string(), false);
    M(store)
}

fn put_store(s: Store) -> M<()> {
    println!("Updating Store: {:?}", s);
    M(())
}

fn put_time(t: Time) -> M<()> {
    unsafe {
        CURRENT_TIME = t;
        M(())
    }
}

fn tick(e: &Exp, kl: KAddr, tau: Time) -> Time {
    tau + 1
}

fn kr(kl: KAddr, ks: &KStore) -> Vec<KAddr> {
    ks.keys().cloned().collect()
}

fn r(e: &Exp, rho: &Env, sigma: &Store, kl: KAddr, ks: &KStore) -> Vec<Atom> {
    sigma.keys().cloned().collect()
}

fn tick_m(e: Exp) -> M<()> {
    bind(get_time(), |tau| 
    bind(get_kaddr(), |kl| {
        let new_time = tick(&e, kl, tau);
        put_time(new_time)
    }))
}

fn gc(e: Exp) -> M<()> {
    bind(get_env(), |rho| 
    bind(get_store(), |sigma| 
    bind(get_kaddr(), |kl| 
    bind(get_kstore(), |ks| {
        let new_ks: KStore = ks.iter()
            .filter(|(k, _)| kr(kl, &ks).contains(k))
            .map(|(k, v)| (*k, v.clone()))
            .collect();
        let new_sigma: Store = sigma.iter()
            .filter(|(l, _)| r(&e, &rho, &sigma, kl, &ks).contains(l))
            .map(|(l, v)| (*l, *v))
            .collect();
        bind(put_kstore(new_ks), |_| 
        put_store(new_sigma))
    }))))
}