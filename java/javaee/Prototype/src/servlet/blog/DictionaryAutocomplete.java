package servlet.blog;

import java.io.IOException;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dictionary.*;
import com.google.gson.*;

public class DictionaryAutocomplete extends HttpServlet {
	private static final long serialVersionUID = 1962439015362375974L;

	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		System.out.println("In Autocomplete servlet. Input was: "
				+ request.getParameter("input"));
		String input = request.getParameter("input");
		int num = Integer.valueOf(request.getParameter("num"));
		List<String> list = Dictionary.getAutocompleteSuggestions(input, num);
		Dictionary.getDictionary();
		Gson a = new Gson();
		String b = a.toJson(list);
		
		boolean first = true;
		response.getWriter().append(b);
		/*
		response.getWriter().append("{ \"list\":[");
		for (String word : list) {
			if (first) {
				first = false;
			} else {
				response.getWriter().append(", ");
			}
			System.out.println("Adding word: " + word);
			response.getWriter().append("{\"word\":\"");
			response.getWriter().append(word);
			response.getWriter().append("\"}");
		}
		response.getWriter().append("]}");*/
		// System.out.println("Exit servlet");
	}
}