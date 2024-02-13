open Base
open Lwt

module type Driver = sig
	type 'a io = 'a Lwt.t
    type buffer = Bytes
    type id = int
    type macaddr = string
    type t

    val register : t -> (id, Net.error) result
    val unregister : t -> id -> unit io
    val write : t -> id -> size:int -> (buffer -> int) -> (unit, Net.error) result io
end

module Vnetif (D: Driver) = struct
	type t = {
		name : D.name;
		buffer : D.buffer;
		stats : stats;
		callCounter : int;
	}

	type stats = {
		rx_packets : int32;
		rx_bytes : int64;
		tx_packets : int32;
		tx_bytes : int64;
	}

	module Driver = D

	let create () =
		let listeners = Hashtbl.create 7 in
		let listener_callback f =
			Lwt.async(fun () -> f buffer);
			Lwt.return_unit;
		) in
	{callCounter; listeners; listener_callback}

	let connect () =
		match create with
		| Error _ -> Lwt.fail_with "vnetif: error on creation"
		| Ok id ->
			let stats = {rx_bytes = 0; rx_packets = 0; tx_bytes = 0; tx_packets = 0} in
			let t = {id; size_limit; stats; wake_on_disconnect=None; }

	let mtu t = match t.size_limit with None -> 1500 | Some x -> x

	let write t id ~size fill = 
		let keys = Hashtbl.fold (fun k _v lst -> k::lst) t.listeners [] in
		let send t buf src dst =
			if src != dst then
			begin
				t.callCounter <- t.callCounter + 1;
				let fn = (Hashtbl.find t.listeners dst) in
				let c = (Hashtbl.find t.listener_callbacks_in_progress dst) in
				t.listener_callback fn c (buffer_copy) buf
			end else
			Lwt.return_unit
		in
		let buf = Cstruct.create size in
		let len = fill buf in
		assert (len <= size);
		let buf = Cstruct.sub buf 0 len in
		Lwt_list.iter_s (send t buf id) keys >>= fun () ->
		t.yield () >|= fun () -> Ok()

		t.stats.txBytes <- Int64.add t.stats.txBytes (Int64.of_int size);
		t.stats.txPackets <- Int32.succ t.stats.txPackets;
		>>= fun () ->
		B.write t.backend t.id ~size fill 

end