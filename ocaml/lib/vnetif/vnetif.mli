open Lwt

module type Driver = sig
  type 'a io = 'a Lwt.t
  type buffer = Bytes.t
  type id = int
  type macaddr = string
  type t

  val register : t -> (id, Net.error) result
  val unregister : t -> id -> unit io
  val write : t -> id -> size:int -> (buffer -> int) -> (unit, Net.error) result io
end

module Vnetif (D: Driver) : sig
  type t

  val create : unit -> t
  val connect : unit -> t Lwt.t
  val mtu : t -> int
  val write : t -> id:D.id -> size:int -> (D.buffer -> int) -> (unit, Net.error) result Lwt.t
end
