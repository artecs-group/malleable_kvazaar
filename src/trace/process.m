arg_list = argv();

filename = arg_list{1}
options = arg_list{2}

%source 'v1_rafanoqp.out';
source ( filename );

%plot( fps( :, 1 ), fps( :, 3 ) )

%% No contemplamos el procesamiento de los últimos frames (valores muy altos).
%%plot( fps( 1:end-5, 1 ), fps( 1:end-5, 3 ) )

figure ("visible", "off")
ax = plotyy( fps( 1:end-5, 1 ), fps( 1:end-5, 3 ),  fps( 1:end-5, 1 ), fps( 1:end-5, 2 ) )

%ylim( [ 0 25 ] )
xlabel( 'Frame number' )
ylabel( ax(1), 'Accum. fps' )
ylabel( ax(2), 'Frame encode time (s)' )

title( options )

average = mean( fps( 1:end-5, 3 ) )
maximum = max( fps( 1:end-5, 3 ) )
minimum = min( fps( 1:end-5, 3 ) )

outputfilename = sprintf( '%s.pdf', filename )

%print -dpdf outputfilename

print( outputfilename, '-dpdf' )

