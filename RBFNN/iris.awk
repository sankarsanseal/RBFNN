BEGIN{ 
FS=","
}
{
if( FNR%10!=0)
	{
	print $1, $2, $3 , $4, $5;
	}
}
