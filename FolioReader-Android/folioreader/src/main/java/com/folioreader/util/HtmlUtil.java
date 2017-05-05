package com.folioreader.util;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by xns on 2017/5/5.
 */

public class HtmlUtil {

    public static String htmlByInjectingIntoHead(String html,
                                                 String headHtmToInsert) {
        if (html == null || html.length() == 0 || headHtmToInsert == null
                || headHtmToInsert.length() == 0) {
            return html;
        }

        String pattern = "<head[^>]*>";
        Matcher matcher = Pattern.compile(pattern).matcher(html);

        if (matcher.find()) {
            int headTagEndPos = matcher.end();
            return new StringBuilder(html).insert(headTagEndPos,
                    headHtmToInsert).toString();
        } else {
            return html;
        }
    }
}
