open Lwt

module Vdriver : sig
  type 'a io = 'a Lwt.t
  type buffer = Bytes.t
  type id = int
  type macaddr = string

  type c = {
    mutable callback_counter : t;
    cond : int Lwt_condition.t;
    mutex : Lwt_mutex.t;
  }

  type t = {
    mutable call_counter : int;
    listener_callback : ((buffer -> unit io) -> c -> buffer -> unit io);
    listener_callbacks : (int, c) Hashtbl.t;
    listeners : (int, buffer -> unit io) Hashtbl.t;
  }

  val generate_mac_address : unit -> macaddr
  val create : unit -> t
  val register : t -> (id, Net.error) result
  val unregister : t -> id -> unit io
  val write : t -> id -> size:int -> (buffer -> int) -> (unit, Net.error) result io
end