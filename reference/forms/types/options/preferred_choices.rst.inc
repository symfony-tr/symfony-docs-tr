preferred_choices
~~~~~~~~~~~~~~~~~

**tip**: ``array`` **varsayılan**: ``array()``

Eğer bu seçenek ayaranırsa, bu seçenekteki değer seçim listesinin en üstü'ne
kaydırılacaktır. Aşağıdaki örnekte "Baz" seçeneği bir ayraç ile birlikte
geri kalan tüm seçeneklerin üzerinde gösterilecektir::

    $builder->add('foo_choices', 'choice', array(
        'choices' => array('foo' => 'Foo', 'bar' => 'Bar', 'baz' => 'Baz'),
        'preferred_choices' => array('baz'),
    ));

Tercih edilen seçenekler sadece ``select`` elementi ile kullanıldığında çalışır 
(``expanded`` false ise). Tercih edilen seçenekler ve normal seçenekler 
görsel olarak bir dizi tirelerden oluşan bir çizgi ile ayrılır
(örn. ``-------------------``).
Bu ayracı istediğiniz gibi özelleştirebilirsiniz:


.. configuration-block::

    .. code-block:: jinja
    
        {{ form_widget(form.foo_choices, { 'separator': '=====' }) }}

    .. code-block:: php
    
        <?php echo $view['form']->widget($form['foo_choices'], array('separator' => '=====')) ?>
