$(document).ready( function() {
  $('.slider').slick({
    dots: true,
    infinite: true,
    autoplay:true,
    autoplaySpeed:3000,
    pauseOnFocus: false,
    pauseOnHover: false,
    pauseOnDotsHover: false,
    speed: 500,
    fade: true,
    cssEase: 'ease-in',
  });

  $('.product-page-slider').slick({
    dots: true,
    infinite: true,
    autoplay:true,
    autoplaySpeed:3000,
    pauseOnFocus: false,
    pauseOnHover: false,
    pauseOnDotsHover: false,
    speed: 500,
    fade: true,
    cssEase: 'ease-in',
  });

  function getCookie(name) {
    var cookieValue = null;
    if (document.cookie && document.cookie !== '') {
      var cookies = document.cookie.split(';');
      for (var i = 0; i < cookies.length; i++) {
        var cookie = jQuery.trim(cookies[i]);
        // Does this cookie string begin with the name we want?
        if (cookie.substring(0, name.length + 1) === (name + '=')) {
          cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
          break;
        }
      }
    }
    return cookieValue;
  }

  var form_add_product = $('#form_buying_product');

  function ajax_update_cart(data, url, delete_item) {
    var csrftoken = getCookie('csrftoken');
    data["csrfmiddlewaretoken"] = csrftoken;
    data.delete_item = delete_item;
    $.ajax({
      url: url,
      type: 'POST',
      data: data,
      cache: true,
      success: function(data) {
        var cart = $('#dropdown-cart');
        // var cart_table = $('#nav_table_cart');
        cart.html('<div class="cart-container"><table><tbody id="nav_table_cart">');
        var s = '';
        $.each(data["products"], function(key, value) {
          s += '<tr><td style="width:70%"><div class="cart-product-name">'+
            '<a href="product/'+ value["product_id"] +'">' + value["product_name"] +
            '</a></div></td><td style="width:10%">' +
            '<div class="cart-product-price">$' + value["product_price"] + '</div></td>' +
            '<td style="width:10%"><div class="cart-product-count"><span>X' + value["count"] +
            '</span></div></td><td style="width:10%"><div class="cart-product-delete">' +
            '<a href="" class="delete-item" data-product_id="'+ value["product_id"] +
            '"><span class="glyphicon glyphicon-remove"></span></a></div></td></tr>';
        });
        cart.append(s);
        cart.append('</tbody></table></div>');
        $('#products_count').text(data["product_count"]);
        $('#count').val('');
        if (data["product_count"] == 0) {
          cart.append('<li class="dropdown-header">Empty</li>');
        }
        else {
          var str_to_add = '<li><a href="">Subtotal: $' + data["total_price"] + '</a></li>'+
          '<li><a href="/checkout/">View cart</a></li>';
          cart.append(str_to_add);
        }
      },
      error: function() {
        console.log("this product does not exist");
      }
    });
  }

  form_add_product.find($(".btn.btn-primary")).click(function(e) {
    e.preventDefault();
    var url = form_add_product.attr("action");
    btn = $(this);
    var count = $('#count').val();
    data = {};
    data.product_id = btn.data('product_id');
    data.product_name = btn.data('product_name');
    data.product_price = btn.data('product_price');
    if (count) {
      data.count = count;
    }
    else {
      data.count = 1;
    }

    ajax_update_cart(data, url, false);
  });

  $(document).on('click', '.delete-item', function(e) {
    e.preventDefault();
    var url = form_add_product.attr("action");
    var data = {};
    data.product_id = $(this).data("product_id");
    ajax_update_cart(data, url, true);
  });

  $('#dropdown-toggle').on('mouseover', function() {
    $(this).addClass('open');
  });

  $('#dropdown-toggle').on('mouseout', function() {
    $(this).removeClass('open');
  });

  var form_add_comment = $('#form_add_comment');

  function ajax_add_comment(data, url) {
    var csrftoken = getCookie('csrftoken');
    data["csrfmiddlewaretoken"] = csrftoken;
    $.ajax({
      url: url,
      type: 'POST',
      data: data,
      cache: true,
      success: function(data) {
        var hidden_div = $('#inner_comment_div').clone();
        $('#comment_username').text(data['username']);
        $('#comment_date').text(data['date']);
        $('#comment_text').text(data['comment_text']);
        $('#inner_comment_div').removeClass('hidden');
        $('#inner_comment_div').removeAttr('id');
        $('#comment_date').removeAttr('id');
        $('#comment_username').removeAttr('id');
        $('#comment_text').removeAttr('id');

        $('#comment_div').append(hidden_div);
        $('#user_comment_text').val('');
      },
      error: function() {
        console.log("comment adding failed");
      }
    });
  }

  form_add_comment.on('submit', function(e) {
    e.preventDefault();
    var url = form_add_comment.attr('action');
    data = {};
    data.text = $('#user_comment_text').val();

    ajax_add_comment(data, url);
  });
});
