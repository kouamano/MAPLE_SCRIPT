server := proc( sid )
        use Sockets in
                Write( sid, sprintf( "Hello %s on port %d, from %s\r\n",
                        GetPeerHost( sid ), GetPeerPort( sid ), GetHostName() ) )
        end use
end proc:

Sockets:-Serve( 2525, server );
