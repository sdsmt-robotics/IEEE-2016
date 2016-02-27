Break field into two loops. One for city, one for rural.

four victims, six possible locations.

separate function for each victim
	`bool retrieve_victim_n()` would take care of retreiving the victim from start to finish.
	If it fails, return to central point.

No real need for an actual loop

	start_to_cp()
	victim_1()
	victim_2() //each of these returns to cp when finished
	victim_3()
	victim_4()
	cp_to_start()

consider using a status struct defined in main(), and pass it around.

claws closed only when they're holding a victim

victim 4 we rotate on the right wheel ONLY

Should be able to detect the side of a wall fairly accurately.