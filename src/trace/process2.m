arg_list = argv();

filename = arg_list{1}
options = arg_list{2}

%source 'v1_rafanoqp.out';
source ( filename );

%plot( stats( :, 1 ), stats( :, 3 ) )

%% No contemplamos el procesamiento de los últimos frames (valores muy altos).
%%plot( stats( 1:end-5, 1 ), stats( 1:end-5, 3 ) )

figure ("visible", "off")
ax = plotyy( stats( :, 1 ), stats( :, 3 ), stats( :, 1 ), [ stats( :, 4 ), stats( :, 5 ) ] )

%ylim( [ 0 25 ] )
xlabel( 'Frame number' )
ylabel( ax(1), 'Accum. fps' )
ylabel( ax(2), 'Frame encode time (s)' )

title( options )

average = mean( stats( :, 3 ) )
maximum = max( stats( :, 3 ) )
minimum = min( stats( :, 3 ) )

outputfilename = sprintf( '%s.pdf', filename )

%print -dpdf outputfilename

print( outputfilename, '-dpdf' )

