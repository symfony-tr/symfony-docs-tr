.. index::
   single: Forms; Twig form function reference

Twig �ablonlar� Form Fonksiyonlar�
=====================================

Formlar� olu�tururken kullan�labilecek olan t�m Twig fonksiyonlar� bu belgede verilmi�tir.
Birka� de�i�ik fonksiyon bulunmaktad�r ve her biri formun farkl� k�s�mlar�n�n (etiketler, 
hatalar, ara�lar.. vs.) olu�turulmas�ndan sorumludur.

form_label(form.name, label, variables)
---------------------------------------

Verilen alan i�in etiketi olu�turur. �zel olarak g�r�nmesini istedi�iniz bir etiketi
opsiyonel olan ikinci parametrede verebilirsiniz.

.. code-block:: jinja

    {{ form_label(form.name) }}

    {# A�a��daki iki �rnek de yap�sal olarak do�rudur #}
    {{ form_label(form.name, 'Your Name', { 'attr': {'class': 'foo'} }) }}
    {{ form_label(form.name, null, { 'label': 'Your name', 'attr': {'class': 'foo'} }) }}

form_errors(form.name)
----------------------

Verilan alan i�in hatalar� olu�turur.

.. code-block:: jinja

    {{ form_errors(form.name) }}

    {# herhangi "genel" bir hatay� g�ster #}
    {{ form_errors(form) }}

form_widget(form.name, variables)
---------------------------------

Verilan alan i�in HTML ara� ��kt�s�n� olu�turur. E�er b�t�n bir forma ve ya form alanlar� toplulu�una
uygulan�rsa, i�erdi�i her bir form sat�r� olu�turulur.

.. code-block:: jinja

    {# bir ara� olu�turur ve "foo" s�n�f�n� ekler #}
    {{ form_widget(form.name, { 'attr': {'class': 'foo'} }) }}

``form_widget`` i�in belirtilen ikinci parametre bir de�i�kenler dizisidir. 
En �ok kullan�lan de�i�ken ``attr`` dir, HTML arac�na uygulanacak HTML 
�zelliklerini i�erir. Baz� durumlarda, baz� tiplere ait �ablonla ilgili 
ba�ka �zellikler de verilebilir. Bunlar her bir tip i�in ayr�ca a��klanm��t�r.

form_row(form.name, variables)
------------------------------

Verilen alan i�in form sat�r�n� olu�turur. Form sat�rlar� alan�n etiketi, 
hatalar� ve HTML arac�ndan olu�ur.

.. code-block:: jinja

    {# bir alan i�in sat�r� olu�tur ama etiket olarak "foo" g�ster #}
    {{ form_row(form.name, { 'label': 'foo' }) }}

``form_row`` i�in verilen ikinci parametre bir de�i�kenler dizisidir. Symfony 
ile sa�lanan �ablonlarda �stteki �rnek gibi sadece etiketin de�i�tirilmesine
izin verilmi�tir.


form_rest(form, variables)
--------------------------

Verilen formda hen�z olu�turulmam�� t�m alanlar� olu�turur. Bu fonksiyonu 
formunuzda bir yerde mutlaka kullanmak iyi bir fikirdir, ��nk� gizli ve ya
unuttu�unuz alanlar� olu�turacakt�r.

.. code-block:: jinja

    {{ form_rest(form) }}

form_enctype(form)
------------------

E�er form en az bir tane dosya y�kleme alan� i�eriyorsa, bu fonksiyon 
gerekli ``enctype="multipart/form-data"`` form �zelli�ini olu�turur. 
Bu fonksiyonu da form i�inde kullanmak iyi bir fikirdir:

.. code-block:: html+jinja

    <form action="{{ path('form_submit') }}" method="post" {{ form_enctype(form) }}>