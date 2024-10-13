Scene(1, "Lodging Traveler NPC Escort Papaya" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()	
	
	Action( "point move" )
	--[
		Param( "loc x", -3852.417 )
		Param( "loc y", -83.806 )
		Param( "loc z", -5623.668 )
		Param( "run mode", 1 )
	--]
	End()	
	
	Action( "send event to wps" )
	--[	
		Param( "wps index", 3758 )
		Param( "event id", 1 )
	--]
	End()
	
    Action( "sps end" )
	--[
        
    --]
    End()
--]
End()
