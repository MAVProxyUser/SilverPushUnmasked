#!/usr/bin/tclsh
package require Tk

proc sound {} {
	global freq dur argv0
	if [catch {exec [file join [file dirname $argv0] "generator"] $freq $dur} results] {
		tk_messageBox -message "Error: $results"
	}
}

label .freq -text "Frequency: "
entry .fbox -textvariable freq
label .fhz -text "Hz"

label .dur -text "Duration: "
entry .dbox -textvariable dur
label .dsec -text "Sec."

button .go -text "Go" -command sound

set freq 440
set dur 1

grid .freq .fbox .fhz
grid .dur .dbox .dsec
grid .go - -
