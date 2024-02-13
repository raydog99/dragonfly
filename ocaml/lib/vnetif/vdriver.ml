open Base
open Lwt
open Random

module Vdriver = struct
	type 'a io = 'a Lwt.t
	type buffer = Bytes
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

	let generate_mac_address () =
		let random_byte () = Printf.sprintf "%02X" (Random.int 256) in
		let mac_bytes = List.init 6 (fun _ -> random_byte ()) in
		String.concat ":" mac_bytes

	let create () =
		let call_counter = 0 in
		let listeners = Hashtbl.create 7 in
		let macs = Hashtbl.create 7 in
		let listener_callbacks = Hashtbl.create 7 in
		let listener_callback f c buffer =
			Lwt.async(fun () -> f buffer) >>= Lwt.return_unit
			in
		{call_counter;listeners;listener_callbacks;listener_callback}

	let register t =
		Hashtbl.add t.listener_callbacks last_id {
			callback_counter = 0;
			cond = Lwt_condition.create();
			mutex = Lwt_mutex.create() };
		Ok t.last_id

	let unregister t id = 
		Hashtbl.remove t.listeners id;
		Hashtbl.remove t.listener_callbacks id;
		Lwt.return_unit;

	let write t id ~size fill = 
		let keys = Hashtbl.fold (fun k _v lst -> k::lst) t.listeners [] in
		let send t buf src dst = 
			if src != dst then
			begin
				t.call_counter <- t.call_counter + 1;
				let fn = (Hashtbl.find t.listeners dst) in
				let c = (Hashtbl.find t.listener_callbacks dst) in
				t.listener_callback fn c (buffer_copy buf)
			end else
			Lwt.return_unit
		in
		let buf = Cstruct.create size in
		let len = fill buf in
		assert (len <= size);
		let buf = Cstruct.sub buf 0 len in
		Lwt_list.iter_s (send t buf id) keys >>= fun () ->
		t.yield () >|= fun () -> Ok()

end