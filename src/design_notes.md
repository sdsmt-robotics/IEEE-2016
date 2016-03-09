four victims, six possible locations.


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

Look into using a mouse for localization. Would work great for city portion, depends on the mouse for rural.

Look into replacing steppers with geared motors with encoders. Cheap dual motor drivers exist.
