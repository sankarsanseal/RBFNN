BEGIN{ 
FS=" "
}
{
if( FNR%10!=0)
	{
	print  $2, $3 , $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $1;
	}
}
