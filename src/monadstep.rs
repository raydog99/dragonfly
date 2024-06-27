use std::marker::PhantomData;

trait MonadStep<S, M> {
    type A;
    type B;
    fn mstep_gamma<F>(&self, f: F, s: S) -> S
    where
        F: Fn(Self::A) -> M;
}

fn mstep_gamma_p<S, M, F>(p: P<M>, f: F, s: S) -> S
where
    S: MonadStep<S, M>,
    F: Fn(S::A) -> M,
{
    S::mstep_gamma(&s, f, s)
}

struct Id<A>(A);

impl<A> MonadStep<Id<A>, Id<A>> for Id<A> {
    type A = A;
    type B = A;
    fn mstep_gamma<F>(&self, f: F, s: Id<A>) -> Id<A>
    where
        F: Fn(A) -> Id<A>,
    {
        f(s.0)
    }
}

struct StateT<S, M, A>(PhantomData<(S, M, A)>);

impl<S, M, A, B> MonadStep<StateT<S, M, A>, StateT<S, M, B>> for StateT<S, M, A>
where
    M: MonadStep<M, M>,
{
    type A = A;
    type B = B;
    fn mstep_gamma<F>(&self, f: F, s: StateT<S, M, A>) -> StateT<S, M, B>
    where
        F: Fn(A) -> StateT<S, M, B>,
    {
        StateT(PhantomData)
    }
}

struct ListSet<A>(Vec<A>);

struct ListSetT<M, A>(PhantomData<(M, A)>);

impl<M, A, B> MonadStep<ListSetT<M, A>, ListSetT<M, B>> for ListSetT<M, A>
where
    M: MonadStep<M, M> + JoinLattice,
{
    type A = A;
    type B = B;
    fn mstep_gamma<F>(&self, f: F, s: ListSetT<M, A>) -> ListSetT<M, B>
    where
        F: Fn(A) -> ListSetT<M, B>,
    {
        ListSetT(PhantomData)
    }
}

struct ListSetWithTop<A>(Option<Vec<A>>);

struct ListSetWithTopT<M, A>(PhantomData<(M, A)>);

impl<M, A, B> MonadStep<ListSetWithTopT<M, A>, ListSetWithTopT<M, B>> for ListSetWithTopT<M, A>
where
    M: MonadStep<M, M> + JoinLattice + Top,
{
    type A = A;
    type B = B;
    fn mstep_gamma<F>(&self, f: F, s: ListSetWithTopT<M, A>) -> ListSetWithTopT<M, B>
    where
        F: Fn(A) -> ListSetWithTopT<M, B>,
    {
        ListSetWithTopT(PhantomData)
    }
}

trait Commute<T, U> {
    fn commute(t: T) -> U;
}

impl<A> Commute<Id<ListSet<A>>, ListSet<Id<A>>> for Id<A> {
    fn commute(id: Id<ListSet<A>>) -> ListSet<Id<A>> {
        ListSet(id.0.0.into_iter().map(Id).collect())
    }
}

impl<S, A> Commute<(S, ListSet<A>), ListSet<(S, A)>> for (S, A)
where
    S: JoinLattice + Clone,
{
    fn commute(pair: (S, ListSet<A>)) -> ListSet<(S, A)> {
        ListSet(pair.1.0.into_iter().map(|a| (pair.0.clone(), a)).collect())
    }
}

struct Compose<T, U, A>(T, PhantomData<(U, A)>);

impl<T, U, A> Compose<T, U, A>
where
    T: Commute<T, ListSet<A>>,
    U: Commute<U, ListSet<A>>,
    T: Functor,
{
    fn new(t: T) -> Self {
        Compose(t, PhantomData)
    }
}

impl<T, U, A> Commute<Compose<T, U, ListSet<A>>, ListSet<Compose<T, U, A>>> for Compose<T, U, A>
where
    T: Commute<T, ListSet<A>>,
    U: Commute<U, ListSet<A>>,
    T: Functor,
{
    fn commute(compose: Compose<T, U, ListSet<A>>) -> ListSet<Compose<T, U, A>> {
        let t = compose.0;
        let commuted = T::commute(t);
        let mapped = commuted.0.into_iter().map(|u| U::commute(u)).collect::<Vec<_>>();
        ListSet(mapped.into_iter().map(|inner| Compose::new(inner)).collect())
    }
}

struct IsoMonadStep<S1, S2, M, A>(PhantomData<(S1, S2, M, A)>);

impl<S1, S2, M, A> MonadStep<S1, IsoMonadStep<S1, S2, M, A>> for S1
where
    S2: MonadStep<S2, M>,
    S1: Isomorphism2<S2>,
{
    type A = A;
    type B = A;
    fn mstep_gamma<F>(&self, f: F, s: S1) -> S1
    where
        F: Fn(A) -> IsoMonadStep<S1, S2, M, A>,
    {
        let s2 = S1::isoto2(s);
        let result = S2::mstep_gamma(&s2, |a| {
            let iso_m = f(a);
            iso_m.0
        }, s2);
        S1::isofrom2(result)
    }
}

trait JoinLattice {
    fn join(&self, other: &Self) -> Self;
}

trait Top {
    fn top() -> Self;
}

trait Isomorphism2<T> {
    fn isoto2(self) -> T;
    fn isofrom2(t: T) -> Self;
}

trait Functor {
    type A;
    type B;
    fn fmap<F>(&self, f: F) -> Self
    where
        F: Fn(Self::A) -> Self::B;
}

struct P<M>(PhantomData<M>);