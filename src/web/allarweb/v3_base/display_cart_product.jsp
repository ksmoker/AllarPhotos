<%@ include file="/ema_kpg/include/global.inc" %>
<%	addParams.put(RequestConstants.REQUEST_ACTION+"x",ActionConstants.ACTION_GET_PRODUCT);%>
<%@ include file="/ema_kpg/include/actionsAndValidate.inc" %>
<jsp:useBean id="productBean" scope="session" class="com.allarphoto.client.beans.ProductBean"/>
<c:set var="product" value="${productBean.product}"/>
<c:set var="defaultImage" value='${configuration.properties["default_image"]}'/>
<c:set var="thumbDir" value='${product.value["Thumbnail Directory"]}'/>
<c:set var="servletDir" value='${configuration["servlet_dir"]}'/>
        

<%
	String title = productBean.getPrimary().substring(0,productBean.getPrimary().lastIndexOf(".")), banner="browse",help="detail";
%>
<%@ include file="header.txt"%>
	<table cellpadding="0" cellspacing="0" border="0">
	<tr>
		<td rowspan='2'>
			<UL>
				<c:forEach var="field" items="${productBean.productFamily.fields}">
					<c:set var="prodKeyword" value="${product.value[field.name]}"/>
					<c:if test="${field.displayOrder >= 1000 && null!= prodKeyword}" >
						<LI><B><c:out value="${field.name}"/>:</B>&nbsp;<c:out value="${prodKeyword}"/>
					</c:if>
				</c:forEach>
			</UL></td>
		<td width="30" rowspan='2'>
			&nbsp;</td>
		<td align="center">
			<h3><%=title%></h3>

			<% log.debug("start display image "+ productBean.getPrimary());%>
			<c:choose>
				<c:when test='${product.value["Height"] > product.value["Width"]}'>
					<IMG SRC='<c:out value="${servletDir}"/>fetchpix?file=<c:out value="${thumbDir}"/>&mimetype=image/jpeg' BORDER="0" HEIGHT="400"><br>
				</c:when>
				<c:otherwise>
					<IMG SRC='<c:out value="${servletDir}"/>fetchpix?file=<c:out value="${thumbDir}"/>&mimetype=image/jpeg' BORDER="0" WIDTH="400"><br>
				</c:otherwise>
			</c:choose>
			<% log.debug("end display image "+ productBean.getPrimary());%>
			<form action="shopping_cart.jsp">
				<input type="submit" value="Go Back">
			</form></td></tr>
	<tr valign="top">
		<td align="center">		
			<FORM ACTION="shopping_cart.jsp" METHOD="post"> 
				<input type="hidden" name="request_product_id" value='<c:out value="${product.id}"/>'>
				<input type="hidden" name="request_product_family" value='<c:out value="${product.productFamilyName}"/>'>
				<c:choose> 
					<c:when test="${productBean.inCart}">
						<input type="hidden" name="action" value="action_remove_from_cart">
						<input type="hidden" name="removeFromCart.remove" 
										value='<c:out value="${product.id}"/>|<c:out value="${product.productFamilyName}"/>'>
						<input type="submit" align="top" src="buttonImages/removeFromCartButton.gif"
								align="absmiddle" name="submit" value="Remove from Cart" border=0>
					</c:when>
					<c:otherwise>
						<input type="hidden" name="action" value="action_add_to_cart">
						<input type="submit" align="top" src="buttonImages/addToCartButton.gif"
								align="absmiddle" name="submit" value="Add to Cart" border=0>
					</c:otherwise>
				</c:choose>
			</form></td>
	</tr></table>

<%@ include file="footer.jsp"%>