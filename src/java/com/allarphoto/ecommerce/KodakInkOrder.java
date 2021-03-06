package com.allarphoto.ecommerce;

import java.util.Collection;

import com.allarphoto.application.Fulfillment;
import com.allarphoto.beans.OrderItem;

public class KodakInkOrder extends Order {
	private static final long serialVersionUID = 1;

	public KodakInkOrder() {
		super();
	}

	public KodakInkOrder(Merchant merchant, CommerceUser user) {
		super(merchant, user);
	}

	public KodakInkOrder(Collection<OrderItem> items) {
		super(items);
	}

	@Override
	protected Fulfillment createFulfillmentModel() {
		Fulfillment fulfill = new KodakInkFulfillment(getUser());
		initFulfilment(fulfill);
		return fulfill;
	}

}
