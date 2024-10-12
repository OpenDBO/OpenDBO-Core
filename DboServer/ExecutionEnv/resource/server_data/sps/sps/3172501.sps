Scene(1, "Governor Long NPC Escort Papaya" )
--[

	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", -3867.016 )
		Param( "loc y", -84.600 )
		Param( "loc z", -5660.805 )
		Param( "run mode", 1 )
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
		Param( "event id", 4 )
	--]
	End()
	
    Action( "sps end" )
	--[
        
    --]
    End()
--]
End()
