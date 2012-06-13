property_path
~~~~~~~~~~~~~

**tip**: ``any`` **varsayılan**: ``alanın değeri``

Form alanları form'ların domain nesnelerinin sınıf değişkenlerindeki değerleri
gösterirler. Bir form submit edildiğinde submit edilen değer nesneye geri 
döner. 

Eğer bir form alanının okuyup yazdığı nesnenin sınıf değişkenini değiştirmek 
istiyorsanız bunu ``property_path`` seçeneğinde belirtirsiniz. 
Bunun varsayılan değeri form alanının ismidir.

Eğer bu form alanı, nesne içerisinde okunmayacak ya da yazmayacak şekilde görmez
den gelmek istenirse bu durumda ``property_path`` kısmını ``false`` yapılması
gerekir.
