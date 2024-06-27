#pragma once

// 001-099: Connection and server related
#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"
#define RPL_ISUPPORT "005"
#define RPL_BOUNCE "010"

// 200-399: Command responses
#define RPL_TRACELINK "200"
#define RPL_TRACECONNECTING "201"
#define RPL_TRACEHANDSHAKE "202"
#define RPL_TRACEUNKNOWN "203"
#define RPL_TRACEOPERATOR "204"
#define RPL_TRACEUSER "205"
#define RPL_TRACESERVER "206"
#define RPL_TRACESERVICE "207"
#define RPL_TRACENEWTYPE "208"
#define RPL_TRACECLASS "209"
#define RPL_TRACERECONNECT "210"
#define RPL_STATSLINKINFO "211"
#define RPL_STATSCOMMANDS "212"
#define RPL_ENDOFSTATS "219"
#define RPL_UMODEIS "221"
#define RPL_SERVLIST "234"
#define RPL_SERVLISTEND "235"
#define RPL_STATSUPTIME "242"
#define RPL_STATSOLINE "243"
#define RPL_LUSERCLIENT "251"
#define RPL_LUSEROP "252"
#define RPL_LUSERUNKNOWN "253"
#define RPL_LUSERCHANNELS "254"
#define RPL_LUSERME "255"
#define RPL_ADMINME "256"
#define RPL_ADMINLOC1 "257"
#define RPL_ADMINLOC2 "258"
#define RPL_ADMINEMAIL "259"
#define RPL_TRACELOG "261"
#define RPL_ENDOFTRACE "262"
#define RPL_TRYAGAIN "263"

// 300-399: Replies to commands
#define RPL_AWAY "301"
#define RPL_USERHOST "302"
#define RPL_ISON "303"
#define RPL_UNAWAY "305"
#define RPL_NOWAWAY "306"
#define RPL_WHOISUSER "311"
#define RPL_WHOISSERVER "312"
#define RPL_WHOISOPERATOR "313"
#define RPL_WHOISIDLE "317"
#define RPL_ENDOFWHOIS "318"
#define RPL_WHOISCHANNELS "319"
#define RPL_LISTSTART "321"
#define RPL_LIST "322"
#define RPL_LISTEND "323"
#define RPL_CHANNELMODEIS "324"
#define RPL_UNIQOPIS "325"
#define RPL_NOTOPIC "331"
#define RPL_TOPIC "332"
#define RPL_INVITING "341"
#define RPL_SUMMONING "342"
#define RPL_INVITELIST "346"
#define RPL_ENDOFINVITELIST "347"
#define RPL_EXCEPTLIST "348"
#define RPL_ENDOFEXCEPTLIST "349"
#define RPL_VERSION "351"
#define RPL_WHOREPLY "352"
#define RPL_ENDOFWHO "315"
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"
#define RPL_LINKS "364"
#define RPL_ENDOFLINKS "365"
#define RPL_BANLIST "367"
#define RPL_ENDOFBANLIST "368"
#define RPL_INFO "371"
#define RPL_ENDOFINFO "374"
#define RPL_MOTDSTART "375"
#define RPL_MOTD "372"
#define RPL_ENDOFMOTD "376"
#define RPL_YOUREOPER "381"
#define RPL_REHASHING "382"
#define RPL_TIME "391"

// 400-599: Error messages
#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHSERVER "402"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_WASNOSUCHNICK "406"
#define ERR_TOOMANYTARGETS "407"
#define ERR_NOSUCHSERVICE "408"
#define ERR_NOORIGIN "409"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_NOTOPLEVEL "413"
#define ERR_WILDTOPLEVEL "414"
#define ERR_BADMASK "415"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NOMOTD "422"
#define ERR_NOADMININFO "423"
#define ERR_FILEERROR "424"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NICKCOLLISION "436"
#define ERR_UNAVAILRESOURCE "437"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL "442"
#define ERR_USERONCHANNEL "443"
#define ERR_NOLOGIN "444"
#define ERR_SUMMONDISABLED "445"
#define ERR_USERSDISABLED "446"
#define ERR_NOTREGISTERED "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_NOPERMFORHOST "463"
#define ERR_PASSWDMISMATCH "464"
#define ERR_YOUREBANNEDCREEP "465"
#define ERR_YOUWILLBEBANNED "466"
#define ERR_KEYSET "467"
#define ERR_CHANNELISFULL "471"
#define ERR_UNKNOWNMODE "472"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BANNEDFROMCHAN "474"
#define ERR_BADCHANNELKEY "475"
#define ERR_BADCHANMASK "476"
#define ERR_NOCHANMODES "477"
#define ERR_BANLISTFULL "478"
#define ERR_NOPRIVILEGES "481"
#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_CANTKILLSERVER "483"
#define ERR_RESTRICTED "484"
#define ERR_UNIQOPPRIVSNEEDED "485"
#define ERR_NOOPERHOST "491"
#define ERR_UMODEUNKNOWNFLAG "501"
#define ERR_USERSDONTMATCH "502"

// Connection and server related
#define _001 "Welcome to the Internet Relay Network"
#define _002 "Your host is running version"
#define _003 "This server was created"
#define _004 "Server info"
#define _005 "Supported features"
#define _010 "Bounce to another server"

// Command responses
#define _200 "Trace link"
#define _201 "Trace connecting"
#define _202 "Trace handshake"
#define _203 "Trace unknown"
#define _204 "Trace operator"
#define _205 "Trace user"
#define _206 "Trace server"
#define _207 "Trace service"
#define _208 "Trace new type"
#define _209 "Trace class"
#define _210 "Trace reconnect"
#define _211 "Stats link info"
#define _212 "Stats commands"
#define _219 "End of stats"
#define _221 "User mode is"
#define _234 "Server list"
#define _235 "End of server list"
#define _242 "Stats uptime"
#define _243 "Stats O-line"
#define _251 "Luser client"
#define _252 "Luser operator"
#define _253 "Luser unknown"
#define _254 "Luser channels"
#define _255 "Luser me"
#define _256 "Admin me"
#define _257 "Admin location 1"
#define _258 "Admin location 2"
#define _259 "Admin email"
#define _261 "Trace log"
#define _262 "End of trace"
#define _263 "Try again"

// Replies to commands
#define _301 "Away"
#define _302 "User host"
#define _303 "Is on"
#define _305 "Unaway"
#define _306 "Now away"
#define _311 "Whois user"
#define _312 "Whois server"
#define _313 "Whois operator"
#define _317 "Whois idle"
#define _318 "End of whois"
#define _319 "Whois channels"
#define _321 "List start"
#define _322 "List"
#define _323 "List end"
#define _324 "Channel mode is"
#define _325 "Unique operator is"
#define _331 "No topic"
#define _332 "Topic"
#define _341 "Inviting"
#define _342 "Summoning"
#define _346 "Invite list"
#define _347 "End of invite list"
#define _348 "Exception list"
#define _349 "End of exception list"
#define _351 "Version"
#define _352 "Who reply"
#define _315 "End of who"
#define _353 "Name reply"
#define _366 "End of /NAMES list"
#define _364 "Links"
#define _365 "End of links"
#define _367 "Ban list"
#define _368 "End of ban list"
#define _371 "Info"
#define _374 "End of info"
#define _375 "Motd start"
#define _372 "Motd"
#define _376 "End of motd"
#define _381 "You are an operator"
#define _382 "Rehashing"
#define _391 "Time"

// Error messages
#define _401 "No such nick/channel"
#define _402 "No such server"
#define _403 "No such channel"
#define _404 "Cannot send to channel"
#define _405 "You have joined too many channels"
#define _406 "There was no such nickname"
#define _407 "Too many targets"
#define _408 "No such service"
#define _409 "No origin specified"
#define _411 "No recipient given"
#define _412 "No text to send"
#define _413 "No toplevel domain specified"
#define _414 "Wildcard in toplevel domain"
#define _415 "Bad server/host mask"
#define _421 "Unknown command"
#define _422 "Motd file is missing"
#define _423 "No administrative info available"
#define _424 "File error"
#define _431 "No nickname given"
#define _432 "Erroneous nickname"
#define _433 "Nickname is already in use"
#define _436 "Nickname collision"
#define _437 "Unavail resource"
#define _441 "User not in channel"
#define _442 "Not on channel"
#define _443 "User on channel"
#define _444 "No login"
#define _445 "SUMMON has been disabled"
#define _446 "USERS has been disabled"
#define _451 "You have not registered"
#define _461 "Not enough parameters"
#define _462 "You may not reregister"
#define _463 "Your host isn't among the privileged"
#define _464 "Password incorrect"
#define _465 "You are banned from this server"
#define _466 "You will be banned"
#define _467 "Key is already set"
#define _471 "Channel is full"
#define _472 "Unknown mode"
#define _473 "Invite only channel"
#define _474 "Banned from channel"
#define _475 "Bad channel key"
#define _476 "Bad channel mask"
#define _477 "No channel modes"
#define _478 "Ban list is full"
#define _481 "No privileges"
#define _482 "Channel operator privileges needed"
#define _483 "Can't kill server"
#define _484 "Restricted"
#define _485 "Unique operator privileges needed"
#define _491 "No O-lines for your host"
#define _501 "Unknown MODE flag"
#define _502 "Users don't match"
