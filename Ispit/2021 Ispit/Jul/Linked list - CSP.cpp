[NODE::NODE(i:1..N)||HEAD::HEAD||FREE::FREE||START::START]

NODE::NODE[
	int value = ...;
	int next = ...;
	int t, x;

	*[ true ->
		head?hasVal(t) -> [
			value == t -> [
				head!(true, next);
			]
			[]
			value != t -> [
				head!(false, next);
			]
		]
		[]
		head?setNext(x) -> [
			next = x;
		]
		[]
		free?setNext(x) -> [
			next = x;
		]
	]
]

HEAD::HEAD[
	int head = ...;
	int t, next;
	
	*[ start?remove(t) ->
		int curr = head, prev = 0;
		bool found = false;
		
		*[ found == false && curr != 0 ->
			node(curr)!hasVal(t)
			bool ret;
			node(curr)?(found, next);
			[ found == false -> 
				prev = curr;
				curr = next;
			]
		]
		
		[ curr != 0 ->
			[
			prev == 0; ->
				head = next;
			[]
			prev != 0; ->
				node(prev)!setNext(next);
			]
		]
		
		free!add(curr);
	]
]

FREE:FREE[
	int head = ...;
	int curr;
	
	*[ head?add(curr) ->
		node(curr)!setNext(head);
		head = curr;
	]
]

START::START[
	int value = ...;
	head!free(value);
]


---------------------------------------------------------------
Napraviti klasni interfejs?
Prepostavka da su na pocetku vec ulancani procesi, i da se
trazi realizovanje samo stavki u zadatku (brisanje, umetanje u
free listu)









